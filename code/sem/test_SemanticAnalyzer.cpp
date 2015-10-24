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
#include "libraries/catch.hpp"
#include "protocols/SyntaxTree.hpp"

#include <iostream>

TEST_CASE("Test the semantic analyzer", "[test-SemanticAnalyzer]")
{
	using namespace tul::protocols;
	using St = tul::protocols::SyntaxTree;

	// Various building procedures
	auto var = [](St *access, St *globality, St *type, St *declaration)
		-> St *
		{
			return new St(
				CrossTerminal::KEYWORD_VAR,
				{access, globality, type, declaration});
		};
	auto global = [](){ return new St(CrossTerminal::KEYWORD_GLOBAL); };
	auto tprivate = []()
		{ return new St(CrossTerminal::KEYWORD_PRIVATE); };
	auto tpublic = []()
		{ return new St(CrossTerminal::KEYWORD_PUBLIC); };
	auto trestricted = []()
		{ return new St(CrossTerminal::KEYWORD_RESTRICTED); };
	auto type = [](St *modifier, St *type)
		{ return new St(CrossTerminal::TYPE, {modifier, type}); };
	auto eps = [](){ return new St(CrossTerminal::EPSILONATE); };
	auto intu = [](std::size_t size){ St *a = new St(CrossTerminal::PRIMITIVE_UNSIGNED); a->setLexeme(std::to_string(size)); return a; };
	auto intuw = [](std::size_t size){ St *a = new St(CrossTerminal::PRIMITIVE_UNSIGNED_WRAPPED); a->setLexeme(std::to_string(size)); return a; };
	auto ints = [](std::size_t size){ St *a = new St(CrossTerminal::PRIMITIVE_SIGNED); a->setLexeme(std::to_string(size)); return a; };
	auto intsw = [](std::size_t size){ St *a = new St(CrossTerminal::PRIMITIVE_SIGNED_WRAPPED); a->setLexeme(std::to_string(size)); return a; };
	auto namelist = [](const std::string &varname,
		St *expression, St *namelist2)
		{
			St *a = new St(CrossTerminal::DATA_NAMES, {expression, namelist2});
			a->setLexeme(varname);
			return a;
		};
	auto enter = [](St *first, St *tail) { return new St(CrossTerminal::ENTER, {first, tail}); };

	SECTION("Basic validation")
	{
		using namespace tul::protocols;
		// The IR of variables is given
		/*
			using the grammar:
			var ::= access globality type varlist
			varlist ::= name optexpr varlist
				| eps

			To process this, we iterate the top of the tree. For
			each recursive definition, we just call recursive functions.
			Suppose enter has var and an enter node (enter takes 2 children).
			Then, we can process both individually. First the var node.
			This node sets up a table of variables...

			If we have a global/glocal, we need to pass a table of available
			glo(b|c)als down to the classes we are processing.
		*/
		SyntaxTree *top = enter(
			var(
				tpublic(),
				global(),
				type(eps(), intu(10)),
				namelist("test", eps(),
					eps())),
			enter(
				eps(),
				eps()));
		std::cout << top->toString();
	}
}
