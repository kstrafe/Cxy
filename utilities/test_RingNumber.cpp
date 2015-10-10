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
#include "utilities/RingNumber.hpp"

#include <limits>


TEST_CASE("RingNumber must behave in a modulus way", "[test-RingNumber]")
{
	using namespace tul::utilities;
	SECTION("RingNumber standard semantics")
	{
		RingNumber<uint32_t> ring;
		REQUIRE(ring.getNumber() == 0);
		REQUIRE(++ring == 1);
		REQUIRE(++ring == 0);
		REQUIRE(ring.getPrevious() == 1);
		REQUIRE(--ring == 1);
	}
	SECTION("Integer overflow semantics")
	{
		RingNumber<int32_t> ring(std::numeric_limits<int32_t>::max());
		int32_t manual_counter = 1;
		#define CAZE() REQUIRE(manual_counter++ == ++ring);
			CAZE()
			CAZE()
			CAZE()
		#undef CAZE
		ring.setNumber(std::numeric_limits<int32_t>::max() - 1);
		REQUIRE(ring.getNumber() == std::numeric_limits<int32_t>::max() - 1);
		REQUIRE(++ring == std::numeric_limits<int32_t>::max());
		REQUIRE(++ring == 0);
		REQUIRE(--ring == std::numeric_limits<int32_t>::max());
	}
	SECTION("0-9")
	{
		RingNumber<int8_t> ring(9);
		int8_t manual_counter = 1;
		#define CAZE() REQUIRE(manual_counter++ == ++ring);
			for (int i = 0; i < 9; ++i)
				CAZE()
		#undef CAZE
		REQUIRE(--ring == 8);
		REQUIRE(++ring == 9);
	}
	SECTION("Check the exception trigger")
	{
		RingNumber<uint8_t> ring(0);
		REQUIRE(++ring == 0);
		REQUIRE(--ring == 0);
		REQUIRE(ring.getPrevious() == 0);
		REQUIRE(ring.getNumber() == 0);
		REQUIRE_THROWS(ring.setNumber(1));
		ring.setNumber(0);
	}
	SECTION("Check the exception trigger on a normal number")
	{
		RingNumber<uint8_t> ring(5);
		REQUIRE(++ring == 1);
		REQUIRE(--ring == 0);
		REQUIRE(ring.getPrevious() == 5);
		REQUIRE(ring.getNumber() == 0);
		REQUIRE_THROWS(ring.setNumber(6));
		ring.setNumber(0);
	}
}
