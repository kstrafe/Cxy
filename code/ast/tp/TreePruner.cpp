// Copyright © 2015 Kevin Robert Stravers
/*
This file is part of Cxy Compiler Reference Implementation (Cxy CRI).

Cxy CRI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Cxy CRI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cxy CRI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "protocols/CrossTerminalTools.hpp"
#include "TreePruner.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>


namespace tul { namespace tp {

void TreePruner::pruneTree(protocols::ConcreteSyntaxTree *ct)
{
	// Assume ct is the root of the tree.
	// Perform the following actions.
	// For each node, decide which can stay, recurse
	for (protocols::ConcreteSyntaxTree *child : ct->children)
		pruneTree(child);

	ct->children.erase
	(
		std::remove_if
		(
			ct->children.begin(), ct->children.end(),
			[](protocols::ConcreteSyntaxTree *child)
			{
				assert(child != nullptr);
				#define caze(x) child->node_type == protocols::CrossTerminal::x ||
				bool predicate =
					caze(GROUPER_LEFT_BRACE)
					caze(GROUPER_LEFT_PARENTHESIS)
					caze(GROUPER_RIGHT_BRACE)
					caze(GROUPER_RIGHT_BRACKET)
					caze(GROUPER_RIGHT_PARENTHESIS)
					caze(KEYWORD_IF)
					caze(KEYWORD_WHILE)
					caze(KEYWORD_ELSE)
					caze(KEYWORD_WHEN)
					caze(SYMBOL_CARET)
					caze(SYMBOL_COLON)
					caze(SYMBOL_SEMICOLON)
					false;
				#undef caze
				if (predicate)
					delete child;
				return predicate;
			}
		),
		ct->children.end()
	);

	using namespace protocols;
	auto popfirst = [&](CrossTerminal cte) {
		for (std::size_t i = 0; i < ct->children.size(); ++i)
		{
			ConcreteSyntaxTree *child = ct->children.at(i);
			if ( child->node_type == cte) {
				assert(child->children.size() == 1);
				decltype(child) trans_child = child->children.at(0);
				child->children.clear();
				ct->children.at(i) = trans_child;
				delete child;
			}
		}
	};
	// Pull the NO_SEMICOLON and STATEMENT upward
	popfirst(CrossTerminal::NO_SEMICOLON_STATEMENT);
	popfirst(CrossTerminal::STATEMENT);

	auto popup = [&](CrossTerminal left, CrossTerminal right) {
		for (std::size_t i = 0; i < ct->children.size(); ++i) {
			ConcreteSyntaxTree *child = ct->children.at(i);
			if ( child->node_type == left ) {
				for (std::size_t j = 0; j < child->children.size(); ++j) {
					ConcreteSyntaxTree *child2 = child->children.at(j);
					if ( child2->node_type == right ) {
						for (std::size_t x = 0; x < j; ++x)
							child2->children.push_front(child->children.at(x));
						for (std::size_t x = j + 1; x < child->children.size(); ++x)
							child2->children.push_back(child->children.at(x));
						ct->children.at(i) = child2;
						child->children.clear();
						delete child;
					}
				}
			}
		}
	};

	#define popud(x, y) popup(CrossTerminal::x, CrossTerminal::y)
	#define exprpop(x, y) popud(x##_EXPRESSION, SYMBOL_##y); popud(OPTIONAL_##x##_EXPRESSION, SYMBOL_##y)
	exprpop(AND, AMPERSAND__AMPERSAND);
	exprpop(OR, BAR__BAR);
	exprpop(BITWISE_OR, BAR);
	exprpop(BITWISE_XOR, CARET);
	exprpop(BITWISE_AND, AMPERSAND);
	exprpop(EQUALITY, EQUAL__EQUAL);
	exprpop(EQUALITY, EXCLAMATION_MARK__EQUAL);
	exprpop(SHIFT, LESS_THAN__LESS_THAN);
	exprpop(SHIFT, GREATER_THAN__GREATER_THAN);
	exprpop(RELATIONAL, GREATER_THAN);
	exprpop(RELATIONAL, GREATER_THAN__EQUAL);
	exprpop(RELATIONAL, LESS_THAN);
	exprpop(RELATIONAL, LESS_THAN__EQUAL);
	exprpop(ADDITIVE, PLUS);
	exprpop(ADDITIVE, MINUS);
	exprpop(MULTIPLICATIVE, STAR);
	exprpop(MULTIPLICATIVE, FORWARD_SLASH);
	popud(RELATIONAL_OPERATOR, SYMBOL_GREATER_THAN);
	popud(RELATIONAL_OPERATOR, SYMBOL_GREATER_THAN__EQUAL);
	popud(RELATIONAL_OPERATOR, SYMBOL_LESS_THAN);
	popud(RELATIONAL_OPERATOR, SYMBOL_LESS_THAN__EQUAL);

	popud(VAR_OR_STATIC, KEYWORD_VAR);
	popud(VAR_OR_STATIC, KEYWORD_STATIC);
	popud(FUNCTION_NAMES, IDENTIFIER_PACKAGE);
	popud(FUNCTION_NAMES, IDENTIFIER_SUBROUTINE);

	popud(FUNCTION_NAMES, SYMBOL_EQUAL__EQUAL);
	popud(FUNCTION_NAMES, SYMBOL_EXCLAMATION_MARK__EQUAL);
	popud(FUNCTION_NAMES, SYMBOL_FORWARD_SLASH);
	popud(FUNCTION_NAMES, SYMBOL_FORWARD_SLASH__EQUAL);
	popud(FUNCTION_NAMES, SYMBOL_GREATER_THAN);
	popud(FUNCTION_NAMES, SYMBOL_GREATER_THAN__EQUAL);
	popud(FUNCTION_NAMES, SYMBOL_GREATER_THAN__GREATER_THAN);
	popud(FUNCTION_NAMES, SYMBOL_GREATER_THAN__GREATER_THAN__EQUAL);
	popud(FUNCTION_NAMES, SYMBOL_LESS_THAN);
	popud(FUNCTION_NAMES, SYMBOL_LESS_THAN__EQUAL);
	popud(FUNCTION_NAMES, SYMBOL_LESS_THAN__LESS_THAN);
	popud(FUNCTION_NAMES, SYMBOL_LESS_THAN__LESS_THAN__EQUAL);
	popud(FUNCTION_NAMES, SYMBOL_MINUS);
	popud(FUNCTION_NAMES, SYMBOL_MINUS__EQUAL);
	popud(FUNCTION_NAMES, SYMBOL_MINUS__MINUS);
	popud(FUNCTION_NAMES, SYMBOL_PERCENT);
	popud(FUNCTION_NAMES, SYMBOL_PERCENT__EQUAL);
	popud(FUNCTION_NAMES, SYMBOL_PLUS);
	popud(FUNCTION_NAMES, SYMBOL_PLUS__EQUAL);
	popud(FUNCTION_NAMES, SYMBOL_PLUS__PLUS);
	popud(FUNCTION_NAMES, SYMBOL_STAR);
	popud(FUNCTION_NAMES, SYMBOL_STAR__EQUAL);

	popud(NAMES_SYMBOL, SYMBOL_EQUAL__EQUAL);
	popud(NAMES_SYMBOL, SYMBOL_EXCLAMATION_MARK__EQUAL);
	popud(NAMES_SYMBOL, SYMBOL_FORWARD_SLASH);
	popud(NAMES_SYMBOL, SYMBOL_FORWARD_SLASH__EQUAL);
	popud(NAMES_SYMBOL, SYMBOL_GREATER_THAN);
	popud(NAMES_SYMBOL, SYMBOL_GREATER_THAN__EQUAL);
	popud(NAMES_SYMBOL, SYMBOL_GREATER_THAN__GREATER_THAN);
	popud(NAMES_SYMBOL, SYMBOL_GREATER_THAN__GREATER_THAN__EQUAL);
	popud(NAMES_SYMBOL, SYMBOL_LESS_THAN);
	popud(NAMES_SYMBOL, SYMBOL_LESS_THAN__EQUAL);
	popud(NAMES_SYMBOL, SYMBOL_LESS_THAN__LESS_THAN);
	popud(NAMES_SYMBOL, SYMBOL_LESS_THAN__LESS_THAN__EQUAL);
	popud(NAMES_SYMBOL, SYMBOL_MINUS);
	popud(NAMES_SYMBOL, SYMBOL_MINUS__EQUAL);
	popud(NAMES_SYMBOL, SYMBOL_MINUS__MINUS);
	popud(NAMES_SYMBOL, SYMBOL_PERCENT);
	popud(NAMES_SYMBOL, SYMBOL_PERCENT__EQUAL);
	popud(NAMES_SYMBOL, SYMBOL_PLUS);
	popud(NAMES_SYMBOL, SYMBOL_PLUS__EQUAL);
	popud(NAMES_SYMBOL, SYMBOL_PLUS__PLUS);
	popud(NAMES_SYMBOL, SYMBOL_STAR);
	popud(NAMES_SYMBOL, SYMBOL_STAR__EQUAL);

	popud(ACCESS_SPECIFIER, KEYWORD_PUBLIC);
	popud(ACCESS_SPECIFIER, KEYWORD_PRIVATE);
	popud(ACCESS_SPECIFIER, KEYWORD_RESTRICTED);

	popud(METHOD_DECL_OR_DEF, STATEMENT_LIST);
	// If this is an expression, and the second child is an expression, and it has two children, bring that child all the way up.

	// Prune all expressions that have one child or the second child as an epsilonate.
	for (
		std::size_t i = 0;
		i < ct->children.size();
		++i
	) {
		if (
			tul::protocols::CrossTerminalTools::isExpression(
				ct->children[i]->node_type)
			&& (
				(
					ct->children[i]->children.size() == 1
				) || (
					ct->children[i]->children.size() == 2
					&& ct->children[i]->children[1]->node_type
						== protocols::CrossTerminal::EPSILONATE
				)
			)
		) {
			assert(ct->children[i]->children[0]->node_type != protocols::CrossTerminal::EPSILONATE);
			// How do we bring that child up? We must make it a proper child of this node. How do we do this?
			// Detach it from the child:
			protocols::ConcreteSyntaxTree *transitive_child = ct->children[i]->children[0];
			ct->children[i]->children.erase(ct->children[i]->children.begin());
			// Now make sure that that child doesn't get deleted
			for (std::size_t j = 0; j < ct->children[i]->children.size(); ++j)
			{
				//protocols::ConcreteSyntaxTree *child = ct->children[i]->children[j];
				// std::cout << protocols::CrossTerminalTools::toString(child->node_type) << j << std::endl;
				delete ct->children[i]->children[j];
			}
			ct->children[i]->children.clear();
			// delete your own child
			delete ct->children[i];

			// Now assign the transitive child to this child.
			ct->children[i] = transitive_child;
		}
	}
}

}}
