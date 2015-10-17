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
		pruneTree(child);

	ct->children.erase
	(
		std::remove_if
		(
			ct->children.begin(), ct->children.end(),
			[](protocols::ConcreteSyntaxTree *child)
			{
				assert(child != nullptr);
				#define caze(x) child->node_type == protocols::CrossTerminal::x ||
				bool predicate =
					caze(GROUPER_LEFT_BRACE)
					caze(GROUPER_LEFT_PARENTHESIS)
					caze(GROUPER_RIGHT_BRACE)
					caze(GROUPER_RIGHT_BRACKET)
					caze(GROUPER_RIGHT_PARENTHESIS)
					caze(KEYWORD_IF)
					caze(KEYWORD_WHILE)
					caze(KEYWORD_ELSE)
					caze(KEYWORD_WHEN)
					caze(SYMBOL_CARET)
					caze(SYMBOL_COLON)
					caze(SYMBOL_SEMICOLON)
					false;
				#undef caze
				if (predicate)
					delete child;
				return predicate;
			}
		),
		ct->children.end()
	);

	// pull no semicolon statement
	/*
	for (std::size_t i = 0; i < ct->children.size(); ++i)
	{
		protocols::ConcreteSyntaxTree *child = ct->children.at(i);
		if (ct->node_type == protocols::CrossTerminal::NO_SEMICOLON_STATEMENT)
		{
			decltype(child) trans_child = child->children.at(0);
			*ct->children.at(i) = *trans_child;
			delete child;
			ct->children.erase(ct->children.begin() + i);
			--i;
		}
	}
	*/

	// Prune all expressions that have one child and the second child as an epsilonate.
	for
	(
		std::size_t i = 0;
		i < ct->children.size();
		++i
	)
	{
		if (
			tul::protocols::CrossTerminalTools::isExpression(
				ct->children[i]->node_type)
			&& (
				(
					ct->children[i]->children.size() == 1
				) || (
					ct->children[i]->children.size() == 2
					&& ct->children[i]->children[1]->node_type
						== protocols::CrossTerminal::EPSILONATE
				)
			)
		) {
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
