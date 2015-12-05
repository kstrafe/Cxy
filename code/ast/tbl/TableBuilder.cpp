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
#include "protocols/CrossTerminalTools.hpp"
#include "TableBuilder.hpp"

#include <cassert>
#include <iostream>


namespace tul { namespace tbl {

// This one gets a tree for a single class/file.
/*
	Let's think about how to process that tree into a table.
	First of all, we need to create a list of grants. Let's do that.
	Also, we know that the last child of enter is the next 'sub part'.
*/
bool TableBuilder::runOn (
	protocols::SyntaxTree *ct_root,
	const std::string &qualified_name
) {
	using namespace protocols;
	#define nodeIs(x) ct_root->node_type == CrossTerminal::x
	#define childNIs(x, n) ct_root->children.at(n)->node_type == CrossTerminal::x
	#define childNLex(n) ct_root->children.at(n)->token.accompanying_lexeme
	#define child1Is(x) childNIs(x, 0)
	#define child2Is(x) childNIs(x, 1)
	#define child3Is(x) childNIs(x, 2)
	#define child2Lex childNLex(1)
	#define last (ct_root->children.end() - 1)
	#define lastIs(x) (*(last))->node_type == CrossTerminal::x
	if (nodeIs(ENTER)) {
	}
	if (ct_root->children.size() > 0)
		if (lastIs(ENTER)) {
			runOn(*last, qualified_name);
		}
	#undef child3Is
	#undef child2Is
	#undef child1Is
	#undef childNIs
	#undef nodeIs
}

}}
