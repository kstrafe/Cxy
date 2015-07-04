#pragma once


#include "protocols/TokenType.hpp"
#include <string>

namespace tul
{
	namespace lexer
	{
		namespace dependency
		{
      class SymbolMatcher
      {
      public:

  			static protocols::TokenType getSymbol(const std::string &lexeme);

      };
		}
	}
}
