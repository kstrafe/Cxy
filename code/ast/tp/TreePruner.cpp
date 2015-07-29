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
#include "protocols/CrossTerminalTools.hpp"
#include "TreePruner.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>


namespace tul { namespace tp {

void TreePruner::pruneTree(protocols::ConcreteSyntaxTree *ct_)
{
	// Assume ct_ is the root of the tree.
	// Perform the following actions.
	// For each node, decide which can stay, recurse
	for (protocols::ConcreteSyntaxTree *child_ : ct_->children_)
	{
		pruneTree(child_);
	}

	ct_->children_.erase
	(
		std::remove_if
		(
			ct_->children_.begin(), ct_->children_.end(),
			[](protocols::ConcreteSyntaxTree *child_)
			{
				assert(child_ != nullptr);
				bool predicate_ =
					// child_->node_type == tul::protocols::CrossTerminal::EPSILONATE
					/* || */  (child_->token_.entry_type == tul::protocols::EntryType::OTHER_SYMBOL
					&& tul::protocols::CrossTerminalTools::isUselessSymbol(child_->node_type) == true)
					|| child_->token_.entry_type == tul::protocols::EntryType::GROUPING_SYMBOL
					|| tul::protocols::CrossTerminalTools::isKeyword(child_->node_type) == true;
				if (predicate_)
					delete child_;
				return predicate_;
			}
		),
		ct_->children_.end()
	);
	// if this node now has one child, and this node is an expression, turn this node into its child?
	// How about: if any child of mine has 1 child that is an expression, make it that expression?
	// return;
	for
	(
		std::size_t i_ = 0;
		i_ < ct_->children_.size();
		++i_
	)
	{
		if
		(
			(
				ct_->children_[i_]->children_.size() == 1
				&& tul::protocols::CrossTerminalTools::isExpression(ct_->children_[i_]->node_type)
			)
			||
			(
				ct_->children_[i_]->children_.size() == 2
				&& ct_->children_[i_]->children_[1]->node_type == protocols::CrossTerminal::EPSILONATE
			)
		)
		{
			assert(ct_->children_[i_]->children_[0]->node_type != protocols::CrossTerminal::EPSILONATE);
			// How do we bring that child up? We must make it a proper child of this node. How do we do this?
			// Detach it from the child:
			protocols::ConcreteSyntaxTree *transitive_child = ct_->children_[i_]->children_[0];
			ct_->children_[i_]->children_.erase(ct_->children_[i_]->children_.begin());
			// Now make sure that that child doesn't get deleted
			for (std::size_t j_ = 0; j_ < ct_->children_[i_]->children_.size(); ++j_)
			{
				//protocols::ConcreteSyntaxTree *child_ = ct_->children_[i_]->children_[j_];
				// std::cout << protocols::CrossTerminalTools::toString(child_->node_type) << j_ << std::endl;
				delete ct_->children_[i_]->children_[j_];
			}
			ct_->children_[i_]->children_.clear();
			// delete your own child
			delete ct_->children_[i_];

			// Now assign the transitive child to this child.
			ct_->children_[i_] = transitive_child;
		}
	}
}

}}
