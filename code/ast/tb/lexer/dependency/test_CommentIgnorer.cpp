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
#include "CommentIgnorer.hpp"
#include "utilities/RingNumber.hpp"

#include <cassert>
#include <iostream>
#include <string>


void expectFrom(std::string from_, std::string to_)
{
	using namespace tul::dependency;
	using namespace tul;

	CommentIgnorer com_ign;

	constexpr const std::size_t CYCLE_BUFFER_SIZE = 2;
	utilities::RingNumber<uint8_t> cycle_buffer(CYCLE_BUFFER_SIZE);
	char cycle_array[CYCLE_BUFFER_SIZE];

	std::string result_;

	for (char in_char : from_)
	{
		cycle_array[++cycle_buffer] = in_char;
		uint8_t action_code = com_ign.putOnStack(in_char);
		if (action_code == 1)
			result_.push_back(cycle_array[cycle_buffer.getNumber()]);
		else if (action_code == 2)
		{
			result_.push_back(cycle_array[cycle_buffer.getPrevious()]);
			result_.push_back(cycle_array[cycle_buffer.getNumber()]);
		}
	}
	REQUIRE(to_ == result_);
}


///////////////////////////////////////////////////////////////////////////////
// The API is quite simple. It is demonstrated and tested here.
////////////////////////////////////////////////////////////////////////////////
TEST_CASE("Ignore comments correctly", "[test-CommentIgnorer]")
{
	expectFrom("Hi! /* wow */ there", "Hi!  there");
	expectFrom("Hi! // wow */ there\nyou", "Hi! \nyou");
	expectFrom("Hi! // /*wow */ there\nyou", "Hi! \nyou");
	expectFrom("Hi! \"// /*wow \"*/ there\nyou", "Hi! \"// /*wow \"*/ there\nyou");
	expectFrom
	(
		"a_.callSome(); // Make sure the call works\na_.makeValue(x_: a_.getValue()~val_ + b_);//This comment has no spaces around \"//\"!",
		"a_.callSome(); \na_.makeValue(x_: a_.getValue()~val_ + b_);"
	);
	expectFrom("/**/A", "A");
	expectFrom("//\nA", "\nA");
	expectFrom("\"//A\"", "\"//A\"");
	expectFrom("\"/*A\"", "\"/*A\"");
	expectFrom("\"/*A*/\"", "\"/*A*/\"");
	expectFrom("/*A*/", "");



}
