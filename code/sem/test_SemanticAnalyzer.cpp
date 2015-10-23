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
#include "protocols/ConcreteSyntaxTree.hpp"

#include <iostream>

TEST_CASE("Test the semantic analyzer", "[test-SemanticAnalyzer]")
{
	#define ncst(a) (new ConcreteSyntaxTree(CrossTerminal::a))
	#define var(a, b, c, d, e, f) new ConcreteSyntaxTree(CrossTerminal::KEYWORD_VAR, {a, b, c, d, e, f})
	#define varname(a) ncst(DATA_NAMES)->setLexeme(#a)
	#define apublic ncst(KEYWORD_PUBLIC)
	#define arestricted ncst(KEYWORD_RESTRICTED)
	#define aprivate ncst(KEYWORD_PRIVATE)
	#define aglobal ncst(KEYWORD_GLOBAL)
	#define type(a, b) (new ConcreteSyntaxTree(CrossTerminal::TYPE, {a, b}))
	#define eps ncst(EPSILONATE)
	#define uint(n) ncst(PRIMITIVE_UNSIGNED)->setLexeme(#n)
	SECTION("Basic validation")
	{
		using namespace tul::protocols;
		ConcreteSyntaxTree *top = var(
			apublic,
			aglobal,
			type(eps, uint(4)),
			varname(myname),
			ncst(EPSILONATE),
			ncst(EPSILONATE)
		);
		std::cout << top->toString();
	}
}
