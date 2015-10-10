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
#include "PositionCounter.hpp"
#include "libraries/catch.hpp"

#include <string>


TEST_CASE("Test the position counter", "[test-PositionCounter]")
{
	SECTION("See if the position counter counts normal asci codes correctly.")
	{
		tul::dependency::PositionCounter position_counter;
		for (auto &character : std::string("This is a (very) normal and expected piece of text\nWorry not\nThe future is green."))
		{
			position_counter.countCharacter(character);
		}
		REQUIRE(position_counter.getCurrentColumnNumber() == 21);
		REQUIRE(position_counter.getCurrentLineNumber() == 3);
	}

	SECTION("Now on some non-ascii stuff...")
	{
		tul::dependency::PositionCounter position_counter;
		for (auto &character : std::string("Jeg liker å fôre hunden min."))
		{
			position_counter.countCharacter(character);
		}
		REQUIRE(position_counter.getCurrentColumnNumber() == 29);
		REQUIRE(position_counter.getCurrentLineNumber() == 1);
	}

	SECTION("Repeating non-ascii characters must be accounted for")
	{
		tul::dependency::PositionCounter position_counter;
		for (auto &character : std::string("å"))
		{
			CHECK(static_cast<unsigned char>(character) >= 128);
			position_counter.countCharacter(character);
		}
		REQUIRE(position_counter.getCurrentColumnNumber() == 2);
		REQUIRE(position_counter.getCurrentLineNumber() == 1);
	}

	SECTION("Test counting moonrunes")
	{
		tul::dependency::PositionCounter position_counter;
		for (auto &character : std::string("ᚠᛇᚻ᛫ᛒᛦᚦ"))
		{
			CHECK(static_cast<unsigned char>(character) >= 128);
			position_counter.countCharacter(character);
		}
		REQUIRE(position_counter.getCurrentColumnNumber() == 8);
		REQUIRE(position_counter.getCurrentLineNumber() == 1);
	}

	SECTION("Some character sets mixed together")
	{
		tul::dependency::PositionCounter position_counter;
		for (auto &character : std::string("еднᚠᛇᚻῶσσ᛫ᛒმხσსნაᛦHelloᚦ"))
		{
			position_counter.countCharacter(character);
		}
		REQUIRE(position_counter.getCurrentColumnNumber() == 25);
		REQUIRE(position_counter.getCurrentLineNumber() == 1);
	}

	SECTION("Subramaniya Bharathiyar's poem")
	{
		tul::dependency::PositionCounter position_counter;
		for (auto &character : std::string("யாமறிந்த மொழிகளிலே தமிழ்மொழி போல் இனிதாவது எங்கும் காணோம்"))
		{
			position_counter.countCharacter(character);
		}
		REQUIRE(position_counter.getCurrentColumnNumber() == 58);
		REQUIRE(position_counter.getCurrentLineNumber() == 1);
	}

	SECTION("Some code points that do not follow strict monospace representation")
	{
		tul::dependency::PositionCounter position_counter;
		for (auto &character : std::string("٩(͡๏̯͡๏)۶   ٩(-̮̮̃•̃)."))
		{
			position_counter.countCharacter(character);
		}
		REQUIRE(position_counter.getCurrentColumnNumber() == 23);
		REQUIRE(position_counter.getCurrentLineNumber() == 1);
	}

}
