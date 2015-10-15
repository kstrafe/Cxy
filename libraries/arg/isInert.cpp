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
along with schdl.  If not, see <http://www.gnu.org/licenses/>.
*/


// Headers
#include "libraries/arg/Argument.hpp"


namespace ttl
{

	bool Argument::isInert(const std::string &flag) const
	{
		return (m_inert_flags.find("--" + flag) != m_inert_flags.end());
	}


	bool Argument::isInert(const char flag) const
	{
		std::string tmp = "-";
		tmp += flag;
		return (m_inert_flags.find(tmp) != m_inert_flags.end());
	}

} // Namespace ttl
