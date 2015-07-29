// Copyright © 2015 Kevin Robert Stravers
/*
This file is part of Unnamed-Language Compiler Reference Implementation (ULCRI).

ULCRI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ULCRI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ULCRI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "protocols/CrossTerminalTools.hpp"
#include "SemanticAnalyzer.hpp"

#include <cassert>
#include <iostream>


namespace tul { namespace sma {

/* Implementation thoughts.
 *
 * What this class will get in is a simple concrete
 * syntax tree. This comes from the parser. The question is, how do we semantically
 * analyze tree. Oh, note that, the tree will be pruned by the TreePruner before
 * it gets here. That's pretty nice. The treepruner removes the data that we absolutely
 * do not need.
 *
 * How do we process the tree correctly? Firstly, we need the function definitions.
 * What will they be used for? Suppose we take each function, and put it in a map.
 * Now, this map is of the form <string, ptr Tree>. So we can access the map, and
 * get any function. The string is to be the complete function signature. Is this
 * a good idea? Let's look at it from a use-point of view.
 *
 * Whenever we access a function or call a function, we will have to look it up in
 * the map. So say we have the code
 *
 *    sml.FunClass.doStuff();
 *
 * This will be separated into a small subtree of an expression. It will be easiest
 * if we could segment the string into something else (like a collection of namespace
 * , class-names, and function names. Now, due to the rules of the language, we
 * have to put the absolute path-name of a module in there. This must be calculated
 * from the root. Now, how do we actually scan the entire module?
 *
 * This needs to be thought out and planned. The current model is to first recognize
 * the elements needed, break them up into minimal modules, and utilize them. This
 * allows for easy refactoring since each module does a small thing. Cohesion is
 * high, coupling extremely low.
 *
 * So we need to iterate directories? That may be provided by boost::directory_iterator.
 * Suppose we have a root folder (the working directory from which the compiler is
 * invoked. Then we also have a single target file (let's call it target).
 *
 * This gives us two paths.
 *
 * root/
 * root/target
 *
 * target can be any combination of directories like `code/subs/Main.tul`, or just
 * `Main.tul`. Both respectively give us:
 *
 * `root/code/subs/Main.tul`
 * `root/Main.tul`
 *
 * So how do we go about? Certainly, it would be most beneficial for semantic analysis
 * to have access to all submodules and provided modules, as well as glocal or global
 * data. The need for this arises during typechecking and other semantic checking.
 * Let's look at how the current model can support that.
 *
 * Up until now, we've been concerned only with the compilation of a single file.
 * Lexing, parsing, tree pruning, and finally semantic analysis. The reason to start
 * looking at other files is simply because we need the information. The downside
 * of this is that we may need to make that information "locally global". Another
 * way of doing things is to go back to the semantic primitive defined previously.
 *
 * The semantic primitive is the <string, ptr Tree> map. This can be made into a
 * <string, <ptr Tree, Map>>. This means that each entry has a module, and each map
 * entry has a list of submodules (folders directly below. How does this work with
 * global (root-accessible) parts though? And how does it work with provided parts?
 *
 * Perhaps we need to delay actual semantic analysis until later. You see, we're
 * currently still in the selection phase. The AST generator could just do the following:
 *
 * 1. Collect all function signatures from a module.
 * 2. Collect every statement under that module (just the subtree).
 * 3. Collect the dependency module names.
 *
 * This makes sense as that's all we can really do. A big part will be another class
 * that will handle actual dependencies. Suppose you perform the above actions on
 * 100 different modules, of various nestings. You will get back a collection of
 * maps containing necessary data. Now, once this data is collected, can we check
 * the entire program for correctness... right? I like the idea. Because we've already
 * written everything, now we need to read everything. This will allow seamless multithreading.
 *
 * That sounds like a good idea. This will mean a collection phase before any compilation
 * thought. I would however like the collection to occur based on the modules that
 * are actually in use. Perhaps both can be done to ensure that a directory tree
 * is being used, and warning the user that certain modules are unused. Say we use
 * the user's invocation as a starting point. Now all that's needed is to put the
 * file's contents through the semantic analyzer. We get back a bunch of function
 * names containing statements. I think the semantic analyzer can also perform statement
 * analysis to determine dependent classes. Then we can start truly multithreading.
 * Now we've gotten a list of - say - eight other classes. We again call the semantic
 * analyzer for each of them (from multiple threads). Merge the resulting dependencies,
 * and run again.
 *
 * This model appeals. It's simple. Can true semantic analysis (checking correctness)
 * be performed directly after the first dependency fetching? I think so. Consider
 * that ALL _true_ dependencies of the first file into the compiler will in fact
 * be fully satisfied. This allows us to perform a complete semantic analysis of
 * the first file after having collected information from immediate dependencies.
 *
 * That is a very appealing model. It forces the errors to come up (the errors that
 * programmers make) as early as possible, thus saving time. Ugh,.. I'm probably
 * prematurely optimizing here. Anyways, it seems like a solid model. One that is
 * easy to refactor and break up. That's the most important thing! Diagrams help.
 *
 * (A): Code --> Semant --> Table/Map structure
 * (B): Dependencies --> Semant --> Table/Map structure
 * analyze(A, B)
 * (C): B --> Semant --> Table/Map structure
 * analyze(B, C)
 *
 * So as can be observed, we start by creating tables for the first module and its
 * direct dependencies. We analyze the first module. Then we create tables for all
 * dependencies' dependencies. Then we analyze all dependencies with the dependencies
 * of the dependencies, and so on.
 *
 * So far so good. How far will the analysis go? It's been stated to just create
 * a table of statements. That seems simple enough. What about branches?
 *
 * Perhaps it's best to keep the structure as-is. Just put the function subtree in
 * that table and be done with it. Then analyze the contents of each function. How?
 * Well, suppose you have the entry:
 *
 * <qualifier, Map>
 *
 * This is not enough to allow for easy addressing. We need to be able to go through
 * a map that has both other submaps (dependencies) and module names. We get a map
 * of the form
 *
 * <qualifier, ModuleMap, Submap>
 *
 * An example of this map:
 *
 * <"internal", ["Amod", "Bmod"], "dependency">
 *
 * Seems quite fair. I wonder, should the lexer be able to tell when a new class
 * ought to be checked for? That probably muddles concerns a lot more than necessary.
 * One thing that leaves me so in doubt is that we remove empty nodes in the tree
 * pruner. Maybe the cst should be kept completely. After all, it has a familiar
 * form that is easy to work with. See, when I remove  - say - epsiloned nodes like
 * a node in an function signature, then I need to manually check the node count.
 *
 * By using the CST directly, the parser has ensured us that each node will have
 * any of the productions. That's something rather appealing. Then, for every nest
 * we dive into, we can let those handler functions check if they are on an epsiloned
 * node. If they are, they return something appropriate. I think this is what has
 * had progress stopped for a little while. Of course, other things can in fact be
 * safely pruned. I was pruning the wrong thing!
 *
 * Currently, removing epsilonation does not prune the expressions. There are a
 * lot of those. I notice that the tree does look clearer.
*/
SemanticAnalyzer::SemanticAnalyzer()
{
	sym::ModuleTable mod_tab;
	mod_tab.qualified_name = "root.Main";
	module_symtab.program_symtab.insert(mod_tab);
}

SemanticAnalyzer::~SemanticAnalyzer()
{
	sym::ModuleTable mod_tab;
	mod_tab.qualified_name = "root.Main";
	std::set<sym::ModuleTable>::iterator it = module_symtab.program_symtab.find(mod_tab);
	for (const sym::MethodTable &method_tab : it->method_symtab)
	{
		std::cout << "Registered function: " << method_tab.method_name << std::endl;
		for (auto it : method_tab.return_params)
		{
			std::cout << it.first << " and " << it.second << std::endl;
		}
	}
}

bool SemanticAnalyzer::runOn(protocols::ConcreteSyntaxTree *ct_root)
{
	return collectFunctionInformation(ct_root);
}


bool SemanticAnalyzer::collectFunctionInformation(protocols::ConcreteSyntaxTree *ct_root)
{
	return true;
	using namespace protocols;
	switch (ct_root->node_type)
	{
		case CrossTerminal::ENTER:
			collectFunctionInformationAfterEnter(ct_root);
		break;
		default: assert(false && "Collect function information must begin at the root of a module."); break;
	}
	return true;
}

bool SemanticAnalyzer::collectFunctionInformationAfterEnter(protocols::ConcreteSyntaxTree *ct_root)
{
	return true;
	using namespace protocols;
	switch (ct_root->node_type)
	{
		default: assert(false && "Collect function information must begin at the root of a module."); break;
	}
	return false;
}

/*
bool SemanticAnalyzer::collectOld()
{
	if
	(
		ct_root->node_type == protocols::CrossTerminal::ENTER
		|| ct_root->node_type == protocols::CrossTerminal::DECL_OR_FUNC
		|| ct_root->node_type == protocols::CrossTerminal::FUNCTION_LIST
	)
	{
		for (protocols::ConcreteSyntaxTree *ct_child : ct_root->children_)
		{
			runOn(ct_child);
		}
	}
	else if (ct_root->node_type == protocols::CrossTerminal::FUNCTION_DEFINITION)
	{
		sym::ModuleTable mod_tab;
		mod_tab.qualified_name = "root.Main";
		std::set<sym::ModuleTable>::iterator it = module_symtab.program_symtab.find(mod_tab);
		sym::MethodTable met_tab;
		collectFunctionSignature(ct_root->children_.at(0), met_tab);
		met_tab.method_name = ct_root->children_.at(1)->token_.accompanying_lexeme;
		std::set<sym::MethodTable> &m_tab = const_cast<std::set<sym::MethodTable> &> (it->method_symtab);
		m_tab.insert(met_tab);
	}
	return true;
}*/

std::string getArgumentType(protocols::ConcreteSyntaxTree *ct_root)
{
	switch (ct_root->node_type)
	{
		case protocols::CrossTerminal::PRIMITIVE_UNSIGNED:
		case protocols::CrossTerminal::PRIMITIVE_SIGNED:
			return ct_root->token_.accompanying_lexeme;
		case protocols::CrossTerminal::TYPE:
		case protocols::CrossTerminal::BASIC_TYPE:
			return getArgumentType(ct_root->children_.at(0));
		break;
		default:
			return "";
		break;
	}
}

std::string getArgumentName(protocols::ConcreteSyntaxTree *ct_root)
{
	switch (ct_root->node_type)
	{
		case protocols::CrossTerminal::BASIC_TYPE:
			return
				ct_root->children_.at(0)->token_.accompanying_lexeme
				+ '.' + ct_root->children_.at(1)->token_.accompanying_lexeme;
		case protocols::CrossTerminal::IDENTIFIER_VARIABLE:
		case protocols::CrossTerminal::IDENTIFIER_PACKAGE:
			return ct_root->token_.accompanying_lexeme;
		default:
			if (ct_root->children_.size() == 0) {
				return protocols::CrossTerminalTools::toString(ct_root->node_type);
			}
			return getArgumentName(ct_root->children_.at(0));
	}
}

bool SemanticAnalyzer::collectFunctionSignature(protocols::ConcreteSyntaxTree *ct_root, sym::MethodTable &mod_tab)
{
	assert(ct_root != nullptr);
	switch (ct_root->node_type)
	{
		case protocols::CrossTerminal::ARGUMENT_LIST_AFTER_FIRST:
		{
			if (ct_root->children_.size() == 1) {
				std::vector<std::pair<std::string, std::string>> &rets = mod_tab.return_params;
				std::string name_ = getArgumentName(ct_root->children_.at(0));
				rets.emplace_back("NoType", name_);
			} else {
				std::vector<std::pair<std::string, std::string>> &rets = mod_tab.return_params;
				std::string type_ = getArgumentType(ct_root->children_.at(0));
				std::string name_ = getArgumentName(ct_root->children_.at(1));
				rets.emplace_back(type_, name_);
			}
		}
		break;
		case protocols::CrossTerminal::OPTIONAL_ARGUMENT_LIST:
		case protocols::CrossTerminal::FUNCTION_SIGNATURE:
		case protocols::CrossTerminal::ARGUMENT_LIST:
			for (protocols::ConcreteSyntaxTree *child_ : ct_root->children_)
				collectFunctionSignature(child_, mod_tab);
			break;
		case protocols::CrossTerminal::ARGUMENT:
		{
			std::vector<std::pair<std::string, std::string>> &rets = mod_tab.return_params;
			std::string type_ = getArgumentType(ct_root->children_.at(0));
			std::string name_ = getArgumentName(ct_root->children_.at(1));
			rets.emplace_back(type_, name_);
		}
		break;
		default:
			std::cout
				<< "Encountered unexpected signature element: "
				<< protocols::CrossTerminalTools::toString(ct_root->node_type)
				<< std::endl;
			assert(false);
		break;
	}
	return true;
}

bool collectScopeInformation(protocols::ConcreteSyntaxTree *ct_root)
{
	// This one finds out the different data inside
	// the function.
	return true;
}


}}
