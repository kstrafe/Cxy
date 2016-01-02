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
#include "Argument.hpp"


TEST_CASE("Test the argument parser", "[test-Argument]")
{
	SECTION("Test single flag arguments")
	{
		ttl::Argument arg;
		arg.setInert('p');
		arg.pass("cxyc -fp");
		REQUIRE(arg.getArgument('f') == "");
		REQUIRE(arg.isPassed('p'));
	}
	SECTION("Test multi-flag arguments")
	{
		ttl::Argument arg;
		arg.setInert("preshared");
		arg.pass("cxyc --preshared -f");
		REQUIRE(arg.getArgument('f') == "");
		REQUIRE(arg.isPassed("preshared"));
	}
	SECTION("Test multi-flags")
	{
		ttl::Argument arg;
		arg.pass("cxyc --preshared -f alpha");
		REQUIRE(arg.getArgument('f') == "alpha");
		REQUIRE(arg.isPassed("preshared"));
	}
}

