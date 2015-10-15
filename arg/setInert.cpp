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
#include "arg/Argument.hpp"


namespace ttl
{

	////////////////////////////////////////////////////////////
	void Argument::internalSetInert(const std::string &flag)
	{
		const std::size_t sz = flag.size();
		if (sz > 0)
		{
			if (flag[0] == '-')
			{
				if (sz > 1 && flag[1] == '-')
				{
					m_inert_flags.insert(flag);
				}
				else
				{
					m_inert_flags.insert(std::string("-") + flag[1]);
				}
			}
		}
	}

	////////////////////////////////////////////////////////////
	void Argument::setInert(const std::string &flag)
	{
		std::string tmp("--");
		tmp += flag;
		m_inert_flags.insert(tmp);
	}

	////////////////////////////////////////////////////////////
	void Argument::setInert(const char flag)
	{
		std::string tmp("-");
		tmp.push_back(flag);
		m_inert_flags.insert(tmp);
	}

	////////////////////////////////////////////////////////////
	void Argument::setInert(const std::initializer_list<const char> flags)
	{
		for (auto &x : flags)
			setInert(x);
	}

	////////////////////////////////////////////////////////////
	void Argument::setInert(const std::initializer_list<const std::string> flags)
	{
		for (auto &x : flags)
			setInert(x);
	}

} // Namespace ttl
