#pragma once
#include "protocols/CrossTerminal.hpp"
#include <string>


namespace tul
{
  namespace parser
  {
    namespace dependency
    {
      class CrossTerminalToString
      {
        public:

          static std::string convertToString(protocols::CrossTerminal cross_terminal);

      };
    }
  }
}
