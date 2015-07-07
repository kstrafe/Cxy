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

#include "libraries/LLOneProductionDeducer.hpp"

#include "protocols/CrossTerminal.hpp"
#include "protocols/ParseReturn.hpp"

namespace tul
{
  namespace treebuilder
  {
    namespace parser
    {
      namespace dependency
      {
        /**
          The goal of this class is to set up the parser correctly. It will need
          to add all productions and check if any production is invalid.
          This class also acts as a barrier as to avoid tight coupling, since it
          only accepts crossterminals and nothing more.
        */
        class CrossTerminalParser
        {
        public:

          CrossTerminalParser();

          std::vector<protocols::CrossTerminal> calculateExpectedTokens(const protocols::CrossTerminal stack_top);

          protocols::ParseReturn<protocols::CrossTerminal> parseSymbol(const protocols::CrossTerminal stack_top, const protocols::CrossTerminal &input_element);

        private:

          libraries::LLOneProductionDeducer<protocols::CrossTerminal> ll_parser;

        };
      }
    }
  }
}
