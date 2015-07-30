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

#include "protocols/ConcreteSyntaxTree.hpp"

#include <string>
#include <utility>
#include <vector>


namespace tul { namespace sym {

class Entry
{
private:

	protocols::CrossTerminal
		access_specifier, object_access_specifier;
	std::vector<std::pair<std::string, std::string>>
		method_returns, method_arguments;
	std::string method_identifier;

	protocols::ConcreteSyntaxTree *sub_tree; // May contain statements if this entry is a
	// function

};

}}
