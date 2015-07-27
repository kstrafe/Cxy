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
  for (protocols::ConcreteSyntaxTree *&child_ : ct_->children_)
  {
    if (child_->children_.size() == 1 && tul::protocols::CrossTerminalTools::isExpression(child_->node_type))
    {
      // How do we bring that child up? We must make it a proper child of this node. How do we do this?
      // Detach it from the child:
      protocols::ConcreteSyntaxTree *transitive_child = child_->children_[0];

      // Now make sure that that child doesn't get deleted
      child_->children_.clear();

      // delete your own child
      delete child_;

      // Now assign the transitive child to this child.
      child_ = transitive_child;
    }
  }
}

}}
