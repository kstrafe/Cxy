#pragma once

namespace tul
{
  namespace protocols
  {
    /**
      Used by the LL(1) parser's token stack. This class contains all terminals
      and non-terminals (hence the name cross-terminal).
    */
    enum class CrossTerminal
    {
      // The first two symbols put onto the stack
      // Non Terminals
      ENTER, EXIT,

      // Unknown crossterminal
      NONE,

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
