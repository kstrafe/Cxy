// Copyright © 2015 Kevin Robert Stravers
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

#include "protocols/ConcreteSyntaxTree.hpp"

#include <string>
#include <utility>
#include <vector>


namespace tul { namespace sym {

class Entry
{
public:

	Entry();
	std::string toString();

	protocols::CrossTerminal
		access_specifier, object_access_specifier;

	std::vector<protocols::CrossTerminal>
		type_prefixes; // Stuff like const ref ptr
	std::string module_identifier, class_identifier;

	std::vector<std::pair<std::string, std::string>>
		method_returns, method_arguments;
	std::string method_identifier;

	protocols::ConcreteSyntaxTree *sub_tree; // May contain statements if this entry is a
	// function

};

}}
