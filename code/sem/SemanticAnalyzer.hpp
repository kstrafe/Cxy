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

namespace tul { namespace sem {

class SemanticAnalyzer
{
public:

	////////////////////////////////////////////////////////////
	// \brief Check the tree for validity in consistency
	//
	// Check if the tree of a class is in the standard form.
	// Please use this only to debug a tree generator.
	// After the check, all functions assume that the
	// tree is in its correct form.
	// Note that this function does not check semantics.
	////////////////////////////////////////////////////////////
	bool checkTree(const protocols::SyntaxTree *tree) const;

private:

};

}}
