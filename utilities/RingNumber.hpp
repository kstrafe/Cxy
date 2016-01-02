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
#include <stdexcept>


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
			return maximum_value;
		return current_value - 1;
	}

	void setNumber(COUNTER number)
	{
		if (number >= 0 && number <= maximum_value)
			current_value = number;
		else
			throw std::invalid_argument("utilities::RingNumber::setNumber(" + std::to_string(number) + ") is out of the defined bounds specified by the constructor");
	}

	COUNTER &operator ++ ()
	{
		if (current_value == maximum_value)
			current_value = 0;
		else
			current_value += 1;
		return current_value;
	}

	COUNTER &operator -- ()
	{
		if (current_value == 0)
			current_value = maximum_value;
		else
			current_value -= 1;
		return current_value;
	}

private:

	COUNTER current_value, maximum_value;
};

}}
