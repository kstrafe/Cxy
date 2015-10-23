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
	#define var(a, b, c, d, e) new ConcreteSyntaxTree(CrossTerminal::KEYWORD_VAR, {a, b, c, d, e})
	SECTION("Basic validation")
	{
		using namespace tul::protocols;
		ConcreteSyntaxTree *top = var(
			new ConcreteSyntaxTree,
			new ConcreteSyntaxTree,
			new ConcreteSyntaxTree,
			new ConcreteSyntaxTree,
			new ConcreteSyntaxTree
		);
		std::cout << top->toString();
	}
}
