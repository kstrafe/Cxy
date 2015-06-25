#pragma once

namespace tul
{
  namespace protocols
  {
    enum class TokenType
    {
      IDENTIFIER_CLASS,
      IDENTIFIER_ENUMERATION,
      IDENTIFIER_PACKAGE,
      IDENTIFIER_SUBROUTINE,
      IDENTIFIER_VARIABLE,

      INTEGER_LITERAL,

      PRIMITIVE_SIGNED,
      PRIMITIVE_UNSIGNED,

      UNIDENTIFIED,
    };
  }
}
