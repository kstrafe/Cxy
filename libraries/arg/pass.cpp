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
#include <cstring>
#include <stack>
#include <iostream>
#include <sstream>


namespace ttl
{

	////////////////////////////////////////////////////////////
	void Argument::pass(const std::size_t argc, char *argv[])
	{
		m_path = argv[0];

		std::stack<std::pair<decltype(m_flags_and_parameters)::iterator, bool>> unset_flags;

		std::size_t i = 1;

		for (; i < argc; ++i)
		{
			if (argv[i][0] == '-')
			{
				if (argv[i][1] == '-')
				{
					if (argv[i][2] == '\0') // An end of flags flag
					{
						++i; break;
					}
					else // A multi-char flag
					{
						setMultiCharFlag(argv[i], unset_flags);
					}
				}
				else // A single-char flag
				{
					setSingleCharFlags(argv[i], unset_flags);
				}
			}
			else // A normal argument
			{
				setArgumentOfUnsetUninertFlag(argv[i], unset_flags);
			}
		} // Loop that counts flags

		for (; i < argc; ++i)
		{
			m_operands.emplace_back(argv[i]);
		} // Loop that accumulates all other arguments
	}

	////////////////////////////////////////////////////////////
	void Argument::pass(const std::string &argument)
	{
		std::stringstream ss;
		ss << argument;
		std::vector<std::string> all;
		std::vector<char *> el;
		while (ss.good())
		{
			all.emplace_back();
			ss >> all.back();
			el.push_back(const_cast<char *>(all.back().data()));
		}
		this->pass(all.size(), &el[0]);
	}

} // Namespace ttl
