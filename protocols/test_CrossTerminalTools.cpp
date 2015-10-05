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
#include "CrossTerminalTools.hpp"
#include "libraries/catch.hpp"

#include <iostream>
#include <vector>


TEST_CASE("Cross-terminal to string representation", "[test-CrossTerminal]")
{
	for (int i = 0; i < static_cast<int>(tul::protocols::CrossTerminal::ENUM_END); ++i)
		REQUIRE(tul::protocols::CrossTerminalTools::toString(static_cast<tul::protocols::CrossTerminal>(i)) != "");
	REQUIRE(tul::protocols::CrossTerminalTools::toString(tul::protocols::CrossTerminal::ENUM_END) == "");
}
