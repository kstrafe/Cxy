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
#include "TreePruner.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>


namespace tul { namespace tp {

void TreePruner::pruneTree(protocols::ConcreteSyntaxTree *ct)
{
	// Assume ct is the root of the tree.
	// Perform the following actions.
	// For each node, decide which can stay, recurse
	for (protocols::ConcreteSyntaxTree *child : ct->children)
	{
		pruneTree(child);
	}

	ct->children.erase
	(
		std::remove_if
		(
			ct->children.begin(), ct->children.end(),
			[](protocols::ConcreteSyntaxTree *child)
			{
				assert(child != nullptr);
				bool predicate =
					// child->node_type == tul::protocols::CrossTerminal::EPSILONATE
					/* || */  (child->token.entry_type == tul::protocols::EntryType::OTHER_SYMBOL
					&& tul::protocols::CrossTerminalTools::isUselessSymbol(child->node_type) == true)
					|| child->token.entry_type == tul::protocols::EntryType::GROUPING_SYMBOL
					|| tul::protocols::CrossTerminalTools::isKeyword(child->node_type) == true;
				if (predicate)
					delete child;
				return predicate;
			}
		),
		ct->children.end()
	);
	// if this node now has one child, and this node is an expression, turn this node into its child?
	// How about: if any child of mine has 1 child that is an expression, make it that expression?
	// return;
	for
	(
		std::size_t i = 0;
		i < ct->children.size();
		++i
	)
	{
		if
		(
			(
				ct->children[i]->children.size() == 1
				&& tul::protocols::CrossTerminalTools::isExpression(ct->children[i]->node_type)
			)
			||
			(
				ct->children[i]->children.size() == 2
				&& ct->children[i]->children[1]->node_type == protocols::CrossTerminal::EPSILONATE
			)
		)
		{
			assert(ct->children[i]->children[0]->node_type != protocols::CrossTerminal::EPSILONATE);
			// How do we bring that child up? We must make it a proper child of this node. How do we do this?
			// Detach it from the child:
			protocols::ConcreteSyntaxTree *transitive_child = ct->children[i]->children[0];
			ct->children[i]->children.erase(ct->children[i]->children.begin());
			// Now make sure that that child doesn't get deleted
			for (std::size_t j = 0; j < ct->children[i]->children.size(); ++j)
			{
				//protocols::ConcreteSyntaxTree *child = ct->children[i]->children[j];
				// std::cout << protocols::CrossTerminalTools::toString(child->node_type) << j << std::endl;
				delete ct->children[i]->children[j];
			}
			ct->children[i]->children.clear();
			// delete your own child
			delete ct->children[i];

			// Now assign the transitive child to this child.
			ct->children[i] = transitive_child;
		}
	}
}

}}
