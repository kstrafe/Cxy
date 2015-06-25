#pragma once

#include "EntryType.hpp"
#include "TokenType.hpp"

#include <string>

namespace tul
{
  namespace protocols
  {
    struct Token
    {
      int line;
      EntryType entry_type;
      TokenType token_type;
      std::string string;
    };
  }
}
