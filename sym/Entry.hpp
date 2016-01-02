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

#include "protocols/SyntaxTree.hpp"

#include <string>
#include <utility>
#include <vector>


namespace tul { namespace sym {

using namespace protocols;

class Entry
{
public:

	// How? We need recursive entries.
	// Turn lambdas into anonymous functions with references.
	// Turn normal statements into??
	/*
		How do we do semantic checking?
		For each module, we register a new name.

		Each module has an array of members.
		Each module has an array of grants.
		Each module has an array of aliases.

		Okay, what's next? Well, we need to store the member types.
		We can start by constructing the simplest table:
		[[name, type],..]
		What about functions?
		[[name, type, statements],..]

		What about grants?
		[[ClassName, grantlist],..]

		And alias:
		[[to, from],..]

		Okay, we now have this information. How do we split up the functions for static
		checking?
		Well, we can - after having segmented the module - run the checker on each function.
		How does this work? For each function, go over the list of statements. Use expression
		S-attribution to find out if types match.
	*/
	Entry();
	std::string toString();

	CrossTerminal
		access_specifier, object_access_specifier;

	std::vector<protocols::CrossTerminal>
		type_prefixes;  // Stuff like const ref ptr
	std::string module_identifier, class_identifier;

	std::vector<std::pair<std::string, std::string>>
		method_returns, method_arguments;
	std::string method_identifier;

	SyntaxTree *sub_tree;  // May contain statements if this entry is a
	// function

};

}}
