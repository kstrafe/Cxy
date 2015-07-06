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
#pragma once

#include <vector>

namespace tul
{
  namespace protocols
  {
    /**
      Used as a return structure by LLOneProductionDeducer. The structure contains
      all productions as children as well as the action to take. If the action
      is not CONTINUE then the child_symbols pointer will probably be nullptr.
    */
    template <typename SYMBOL>
    struct ParseReturn
    {
      const std::vector<SYMBOL> *child_symbols;
      enum class Action
      {
        OBSERVE_ERROR, REMOVE_TOP,
        CONTINUE, EPSILONATE
      } desired_action;
    };
  }
}
