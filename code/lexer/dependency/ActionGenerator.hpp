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
