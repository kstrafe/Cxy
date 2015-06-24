#pragma once

#include "libraries/Mealy.hpp"
#include "protocols/Action.hpp"
#include "protocols/Type.hpp"

#include <cstddef>

namespace tul
{
  namespace lexer
  {
    namespace dependency
    {
      class ActionGenerator
      {
      public:

        ActionGenerator();
        protocols::Action computeAction(protocols::Type type);

      private:

        tul::library::Mealy<std::size_t, protocols::Action, protocols::Type> machine;
          static const tul::library::Mealy<std::size_t, protocols::Action, protocols::Type>::Compound table[25];

      };
    }
  }
}
