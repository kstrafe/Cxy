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
#include "libraries/arg/Argument.hpp"


namespace ttl
{

	////////////////////////////////////////////////////////////
	const std::string &Argument::getArgument(const std::string &flag) const
	{
		std::string tmp("--");
		tmp.append(flag);
		return this->getArgumentInternal(tmp);
	}

	////////////////////////////////////////////////////////////
	const std::string &Argument::getArgument(const char flag) const
	{
		std::string tmp("-");
		tmp.push_back(flag);
		return this->getArgumentInternal(tmp);
	}

} // Namespace ttl


