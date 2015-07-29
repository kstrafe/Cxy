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
#include "LLOneProductionDeducer.hpp"
#include "libraries/catch.hpp"

#include <iostream>
#include <stack>
#include <vector>
#include <map>


TEST_CASE("Test the kernel of the LL(1) production transducer", "[test-LL1]")
{
	SECTION("Try parsing a counting grammar of (), where each entry must match and line up.")
	{
		enum Symbols
		{
			LPAREN, RPAREN,
			GROUPER, POSGROUPER,
			END
		};

		tul::libraries::LLOneProductionDeducer<Symbols> ll_parser;
		ll_parser.addRule(GROUPER, LPAREN, {LPAREN, POSGROUPER, RPAREN, POSGROUPER});
		ll_parser.addRule(POSGROUPER, LPAREN, {LPAREN, POSGROUPER, RPAREN, POSGROUPER});
		ll_parser.addEpsilon(POSGROUPER);

		std::stack<Symbols> symbol_stack;

		symbol_stack.emplace(END);
		symbol_stack.emplace(GROUPER);

		std::vector<Symbols> input_symbols
		{LPAREN, LPAREN, LPAREN, RPAREN, RPAREN, LPAREN, RPAREN, RPAREN, END};

		for (Symbols input_symbol : input_symbols)
		{
			using namespace tul::protocols;
			ParseReturn<Symbols> current_return {nullptr, ParseReturn<Symbols>::Action::CONTINUE};
			bool break_out = false;
			do
			{
				current_return = ll_parser.parseSymbol(input_symbol, symbol_stack.top());

				switch (current_return.desired_action)
				{
					case tul::protocols::ParseReturn<Symbols>::Action::CONTINUE:
						symbol_stack.pop();
						for
						(
							std::size_t index_of_symbols = current_return.child_symbols->size() - 1;
							;
							--index_of_symbols
						)
						{
							symbol_stack.push((*(current_return.child_symbols))[index_of_symbols]);
							if (index_of_symbols == 0)
								break;
						}
						break_out = false;
					break;
					case tul::protocols::ParseReturn<Symbols>::Action::EPSILONATE:
						symbol_stack.pop();
						break_out = false;
					break;
					case tul::protocols::ParseReturn<Symbols>::Action::OBSERVE_ERROR:
						std::cout << "ERROR" << std::endl;
						break_out = true;
					break;
					case tul::protocols::ParseReturn<Symbols>::Action::REMOVE_TOP:
						symbol_stack.pop();
						break_out = true;
					break;
					default:
					break;
				}
			}
			while (break_out == false);
		}
	}
}
