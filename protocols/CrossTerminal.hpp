#pragma once

namespace tul
{
  namespace protocols
  {
    /**
      Used by the LL(1) parser's token stack.
    */
    enum class CrossTerminal
    {
      // The first two symbols put onto the stack
      // Non Terminals
      ENTER, EXIT,

      EXPRESSION,
      PRODUCT,
      MAYBE_PRODUCT,

      // Terminals
      NUMBER,
      STAR,
      PLUS,
    };
  }
}
