#pragma once

namespace tul
{
  namespace protocols
  {
    enum class TokenType
    {
      IDENTIFIER_CLASS,
      IDENTIFIER_SUBROUTINE,
      IDENTIFIER_VARIABLE,
      IDENTIFIER_VARIABLE_OR_SUBROUTINE,

      INTEGER_LITERAL,

      PRIMITIVE_SIGNED,
      PRIMITIVE_UNSIGNED,

      UNIDENTIFIED,
    };
  }
}
