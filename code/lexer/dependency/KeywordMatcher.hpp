#pragma once

#include "protocols/TokenType.hpp"
#include <string>


namespace tul
{
  namespace lexer
  {
    namespace dependency
    {
      class KeywordMatcher
      {
      public:

        static protocols::TokenType getKeyword(const std::string &lexeme);

      };
    }
  }
}
