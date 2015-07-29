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
#pragma once


namespace tul { namespace utilities {

template <typename COUNTER>
class RingNumber
{
public:

	RingNumber()
	:
		current_value(0),
		maximum_value(1)
	{}

	RingNumber(COUNTER max_val)
	:
		current_value(0),
		maximum_value(max_val)
	{}

	const COUNTER &getNumber() const
	{
		return current_value;
	}

	COUNTER &getNumber()
	{
		return current_value;
	}

	COUNTER getPrevious() const
	{
		if (current_value == 0)
			return maximum_value - 1;
		return current_value - 1;
	}

	COUNTER &operator ++ ()
	{
		current_value += 1;
		if (!(current_value < maximum_value))
			current_value = 0;
		return current_value;
	}

	COUNTER &operator -- ()
	{
		if (current_value == 0)
			current_value = maximum_value - 1;
		return current_value;
	}

private:

	COUNTER current_value, maximum_value;
};

}}
