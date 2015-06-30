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
