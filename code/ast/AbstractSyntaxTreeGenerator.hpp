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

#include "tbl/TableBuilder.hpp"
#include "tb/TreeBuilder.hpp"


namespace tul { namespace ast {

class AbstractSyntaxTreeGenerator
{
public:

	bool buildTree(const std::string &code);
	bool buildTree(char next_character);
	bool endInput();
	std::vector<std::string> getExpectedTokens();
	bool generateSemantics(const std::string &qualified_name);

private:

	tb::TreeBuilder tree_builder;
	tbl::TableBuilder table_builder;

};

}}
