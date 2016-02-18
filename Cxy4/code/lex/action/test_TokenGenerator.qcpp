// Copyright © 2015-2016 Kevin Robert Stravers
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
#include "protocols/Action.hpp"
#include "TokenGenerator.hpp"

#include <string>


TEST_CASE("Test the token generator", "[test-TokenGenerator]")
{
	using namespace tul::protocols;
	tul::dependency::TokenGenerator token_generator;
	SECTION("No tokens ought to be put on the stack")
	//enum class Action { N, E, P, PTG, TAPTG, TA, TAP, TRP, TR, TRPTG, TSP, TSPTG, TS  };
	{
		#define cons(action) token_generator.consumeCharacter('a', Action::action)
		for (int i = 0; i < 100; ++i)
		{
			cons(N);
			cons(E);
			if (i % 3 == 0)
				cons(P);
			cons(E);
			if (i % 5 == 0)
				cons(N);
			cons(P);
		}
		#undef cons
		REQUIRE(token_generator.getTokenStack().size() == 0);
	}
	SECTION("Suddenly we get a massive almost 600-char token of 'a'")
	{
		REQUIRE(token_generator.consumeCharacter(' ', Action::TA) == 1);
		REQUIRE(token_generator.getTokenStack().size() == 1);
	}
	SECTION("For some reason the client requests only transfers! Let's see if it holds up...")
	{
		for (int i = 0; i < 100; ++i)
		{
			REQUIRE(token_generator.consumeCharacter(static_cast<char>(i % 256), Action::TAP) == 1);
		}
		REQUIRE(token_generator.getTokenStack().size() == 100 + 1 - 1); // Remember +1 from previous, but also remember that the last one pushed is not transfered
	}
}
