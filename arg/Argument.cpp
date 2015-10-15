/*
Copyright 2013, 2014 Kevin Robert Stravers

This file is part of TTL.

TTL is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

TTL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
ERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TTL.  If not, see <http://www.gnu.org/licenses/>.
*/


// Headers
#include "arg/Argument.hpp"


namespace ttl
{

	Argument::Argument()
	{

	}


	Argument::Argument(const std::size_t argc, char *argv[])
	{
			this->pass(argc, argv);
	}


	Argument::Argument(const std::string &argument)
	{
			this->pass(argument);
	}


	Argument::~Argument()
	{

	}

} // Namespace ttl
