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
#pragma once
#include "utilities/RingNumber.hpp"


namespace tul { namespace stringfilter {

class StringFilter
{
public:

	StringFilter();
	uint8_t hexToBits(char hex);
	void push(char character);
	bool available() const;
	char pop();
	void end();

private:

	enum class State
	{
		NOTHING,
		SINGLE,
		MULTIPLE,
		VERBATIM,
		ESCAPE,
		CODE,
		CODE2,
		ESCAPE_VERBATIM,
	} state = State::NOTHING, previous = State::NOTHING;

	uint8_t out[2], can_fetch = 0;

};

}}
