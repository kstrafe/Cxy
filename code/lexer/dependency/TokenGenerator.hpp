#include <vector>
#include <string>
#include "protocols/Token.hpp"
#include "protocols/Action.hpp"

namespace tul
{
  namespace lexer
  {
    namespace dependency
    {
      class TokenGenerator
      {
      public:

        bool consumeCharacter(char character, protocols::Action action);
        std::vector<protocols::Token> &getTokenStack();

      private:

        std::vector<protocols::Token> tokens;
        std::string        current_working_lexeme;

      };
    }
  }
}
