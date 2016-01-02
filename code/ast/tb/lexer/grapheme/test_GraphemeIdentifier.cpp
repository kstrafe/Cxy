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
#include "../dependency/PositionCounter.hpp"
#include "GraphemeIdentifier.hpp"
#include <libraries/catch.hpp>


namespace {

tul::dependency::PositionCounter counter;
std::size_t last_column = 1;
std::size_t last_index = 0;
union Codepoint
{
	char characters[4];
	uint32_t code_point;
} code_pt;

uint32_t getCodepoint(char byte)
{
	counter.countCharacter(byte);
	if (last_column != counter.getCurrentColumnNumber())
	{
		last_column = counter.getCurrentColumnNumber();
		last_index = 0;
		return code_pt.code_point;
	}
	else
	{
		code_pt.characters[last_index] = byte;
		++last_index;
	}
	return 0;
}

}


TEST_CASE("Ensure the correct function of the grapheme identifier")
{
	tul::grapheme::GraphemeIdentifier graph;
	REQUIRE(graph.isBreakingCodepoint(103));  // 103 == LATIN SMALL LETTER G
	REQUIRE(graph.isBreakingCodepoint(776) == false);  // 776 == COMBINING DIAERESIS
}
