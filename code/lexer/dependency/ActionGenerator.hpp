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

#include "libraries/Mealy.hpp"
#include "protocols/Action.hpp"
#include "protocols/EntryType.hpp"

#include <cstddef>

namespace tul
{
  namespace lexer
  {
    namespace dependency
    {
      /**
        The action generator receives character types. Based on this information,
        it advances a state machine (a mealy machine) and returns the action that
        the edge of the transition is annotated with.

        For example, on the input:
        aa b c
        it will first say "push, push, transfer" on 'aa ' (note the space). This
        means that a lexeme storage is allocated for each push and the character
        is put onto that storage. The transfer says that the token is finished.

        This class solely concerns itself with providing information about what
        action to take.
      */
      class ActionGenerator
      {
      public:

        ActionGenerator();
        protocols::Action computeAction(protocols::EntryType with_type);

      private:

        tul::library::Mealy<std::size_t, protocols::Action, protocols::EntryType> mealy_machine;
        static const tul::library::Mealy<std::size_t, protocols::Action, protocols::EntryType>::Compound action_table[];

      };
    }
  }
}
