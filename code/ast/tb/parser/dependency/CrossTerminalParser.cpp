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
#include "CrossTerminalParser.hpp"


namespace tul { namespace dependency {

CrossTerminalParser::CrossTerminalParser()
{
	// Set up parse table
	using namespace protocols;
	#define cT(name) CrossTerminal::name
	#define eps(left) ll_parser.addEpsilon(CrossTerminal::left)
	#define add(left, middle) \
		if (ll_parser.doesRuleExist(CrossTerminal::left, CrossTerminal::middle)) throw std::string(#left) + " already has transition " + std::string(#middle); \
		ll_parser.addRule(CrossTerminal::left, CrossTerminal::middle,

		#include "CrossTerminalParser.inc"

	#undef add
	#undef eps
	#undef cT
}

std::vector<protocols::CrossTerminal> CrossTerminalParser::calculateExpectedTokens(const protocols::CrossTerminal stack_top)
{
	return ll_parser.getTransitionTokens(stack_top);
}

protocols::ParseReturn<protocols::CrossTerminal> CrossTerminalParser::parseSymbol
(const protocols::CrossTerminal stack_top, const protocols::CrossTerminal &input_element)
{
	using namespace protocols;
	return ll_parser.parseSymbol(input_element, stack_top);
}

}}
