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
#include "ActionGenerator.hpp"
#include "protocols/Action.hpp"
#include "protocols/EntryType.hpp"

#include "libraries/Mealy.hpp"

#include <cstddef>

namespace tul
{
  namespace treebuilder
  {
    namespace lexer
    {
      namespace dependency
      {
        protocols::Action ActionGenerator::computeAction(protocols::EntryType with_type)
        {
          return mealy_machine.transist(with_type, 6);
        }

        ActionGenerator::ActionGenerator()
        {
          /*
            n = no action
            e = error, stray unknown outside a string literal
            p = push current OTHER_SYMBOL on to the current token
            tx = transfer something of type x
            tg = transfer a GROUPING_SYMBOL token
            tr = transfer a string token
            ts = transfer a OTHER_SYMBOL token
            ta = transfer an alnumu token
            txp = transfer x and then push
            ptx = push and then transfer
          */
          mealy_machine.setTransitionTable(action_table);
        }

        using namespace protocols;

        constexpr const tul::library::Mealy<std::size_t, protocols::Action, protocols::EntryType>::Compound ActionGenerator::action_table[]
          { /*    ALPHA_DIGIT_OR_UNDERSCORE  GROUPING_SYMBOL     QUOTE_SYMBOL     OTHER_SYMBOL      UNKNOWN_CODE_POINT  WHITESPACE  */
            /*0*/ {1, Action::P},            {0, Action::PTG},   {2, Action::N},  {4, Action::P},   {0, Action::E},     {0, Action::N},
            /*1*/ {1, Action::P},            {0, Action::TAPTG}, {2, Action::TA}, {4, Action::TAP}, {0, Action::E},     {0, Action::TA},
            /*2*/ {2, Action::P},            {2, Action::P},     {3, Action::N},  {2, Action::P},   {2, Action::P},     {2, Action::P},
            /*3*/ {1, Action::TRP},          {0, Action::TRPTG}, {2, Action::P},  {4, Action::TRP}, {0, Action::E},     {0, Action::TR},
            /*4*/ {1, Action::TSP},          {0, Action::TSPTG}, {2, Action::TS}, {4, Action::P},   {0, Action::E},     {0, Action::TS}
            // Remember to set the transist value in computeAction to the amount of columns here.
          };
      }
    }
  }
}
