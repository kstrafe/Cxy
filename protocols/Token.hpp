#pragma once

#include "Type.hpp"
#include "TokenType.hpp"

#include <string>

namespace tul
{
  namespace protocols
  {
    struct Token
    {
      int line;
      Type entry_type;
      TokenType token_type;
      std::string string;
    };
  }
}
