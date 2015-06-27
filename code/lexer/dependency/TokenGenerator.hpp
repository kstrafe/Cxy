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

        std::size_t consumeCharacter(char character_, protocols::Action action_);
        std::vector<protocols::Token> &getTokenStack();

      private:

        std::vector<protocols::Token> token_stack;
        std::string        current_working_lexeme;

      };
    }
  }
}
