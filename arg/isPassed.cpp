/*
Copyright 2013, 2014 Kevin Robert Stravers

This file is part of TTL.

TTL is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

TTL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TTL.  If not, see <http://www.gnu.org/licenses/>.
*/


// Headers
#include "arg/Argument.hpp"


namespace ttl
{

	////////////////////////////////////////////////////////////
	bool Argument::isPassed(const std::string &argument) const
	{
		std::string tmp = argument;
		tmp.insert(0, "--");

		return (m_flags_and_parameters.find(tmp) != m_flags_and_parameters.end());
	}

	////////////////////////////////////////////////////////////
	bool Argument::isPassed(const char argument) const
	{
		std::string tmp("-");
		tmp.push_back(argument);

		return (m_flags_and_parameters.find(tmp) != m_flags_and_parameters.end());
	}

} // Namespace ttl
