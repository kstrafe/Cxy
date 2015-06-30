#pragma once

#include "libraries/LLOneProductionDeducer.hpp"

#include "protocols/CrossTerminal.hpp"
#include "protocols/ParseReturn.hpp"

namespace tul
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

        protocols::ParseReturn<protocols::CrossTerminal> parseSymbol(const protocols::CrossTerminal &input_element);

      private:

        libraries::LLOneProductionDeducer<protocols::CrossTerminal> ll_parser;

      };
    }
  }
}
