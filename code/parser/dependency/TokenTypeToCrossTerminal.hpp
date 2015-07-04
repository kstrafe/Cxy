#pragma once

#include "protocols/CrossTerminal.hpp"
#include "protocols/TokenType.hpp"

namespace tul
{
	namespace parser
	{
		namespace dependency
		{
      class TokenTypeToCrossTerminal
      {

      public:

  			static protocols::CrossTerminal convertToCrossTerminal(protocols::TokenType token_type);

      };
		}
	}
}
