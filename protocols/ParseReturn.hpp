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
