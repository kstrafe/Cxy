#include <vector>
#include <string>
#include "protocols/Action.hpp"
#include "protocols/Token.hpp"

#include <cstddef>

namespace tul
{
  namespace lexer
  {
    namespace dependency
    {
      class TokenGenerator
      {
      public:

        std::size_t consumeCharacter(char character, protocols::Action action);
        std::vector<protocols::Token> &getTokenStack();

      private:

        std::vector<protocols::Token> tokens;
        std::string        current_working_lexeme;

      };
    }
  }
}
