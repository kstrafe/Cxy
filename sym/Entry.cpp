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
#include "Entry.hpp"

#include <string>


namespace tul { namespace sym {

Entry::Entry()
:
	access_specifier(protocols::CrossTerminal::EPSILONATE),
	object_access_specifier(protocols::CrossTerminal::EPSILONATE)
{}

std::string Entry::toString()
{
	using namespace protocols;
	std::string str_rep = "============================\n";
	str_rep += CrossTerminalTools::toString(access_specifier);
	str_rep += ':';
	str_rep += CrossTerminalTools::toString(object_access_specifier);

	std::string str_type_prefixes = "\n";
	for (CrossTerminal ct_ : type_prefixes)
	{
		str_type_prefixes += CrossTerminalTools::toString(ct_);
		str_type_prefixes += "\n";
	}

	return
		str_rep + "\n" +
		method_identifier + "\n" +
		str_type_prefixes + "\n" +
		"============================\n";
}

}}
