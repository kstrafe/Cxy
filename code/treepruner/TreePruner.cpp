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

namespace tul {

void TreePruner::pruneTree(protocols::ConcreteSyntaxTree *ct_)
{
  // Assume ct_ is the root of the tree.
  // Perform the following actions.
  // For each node, decide which can stay, recurse
  ct_->children_.erase
  (
    std::remove_if
    (
      ct_->children_.begin(), ct_->children_.end(),
      [](protocols::ConcreteSyntaxTree *child_)
      {
        assert(child_ != nullptr);
        bool predicate_ =
          child_->node_type == tul::protocols::CrossTerminal::EPSILONATE
          || child_->token_.entry_type == tul::protocols::EntryType::OTHER_SYMBOL
          || child_->token_.entry_type == tul::protocols::EntryType::GROUPING_SYMBOL
          || tul::protocols::CrossTerminalTools::isKeyword(child_->node_type) == true;
        if (predicate_)
          delete child_;
        return predicate_;
      }
    ),
    ct_->children_.end()
  );

  for (protocols::ConcreteSyntaxTree *child_ : ct_->children_)
  {
    pruneTree(child_);
  }

}

}
