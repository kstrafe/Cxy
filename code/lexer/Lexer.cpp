#include "Lexer.hpp"
#include "protocols/Token.hpp"

#include <algorithm>
#include <vector>

namespace tul
{
  namespace lexer
  {
    void Lexer::insertCharacter(char character)
    {
      protocols::Type type_of_character = typify(character);
      protocols::Action action_to_perform = action_generator.computeAction(type_of_character);
      if (token_generator.consumeCharacter(character, action_to_perform))
      {
        // A new token is ready to be identified
        identifyToken(getTokenStack().back());
      }
    }

    std::vector<protocols::Token> &Lexer::getTokenStack()
    {
      return token_generator.getTokenStack();
    }

    void Lexer::identifyToken(protocols::Token &token)
    {
      using namespace protocols;

      // enum class Type { alphanumu, grouper, quote, symbol, whitespace };

      if (token.entry_type == Type::alphanumu)
      {
        // We know it's either a Class name, function, variable, number, or primitive
        if (std::isupper(token.string.at(0)))
        {
          // Bingo, class name!
          token.token_type = TokenType::IDENTIFIER_CLASS;
        }
        else if (std::islower(token.string.at(0)))
        {
          // If all characters are lower: var or sub identifier
          if (std::all_of(token.string.cbegin() + 1, token.string.cend(), [](char character) -> bool {return std::islower(character);}))
          {
            token.token_type = TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE;
          }
        }
        else if (std::isdigit(token.string.at(0)))
        {
          // Check if last is u or s
          if (std::isdigit(token.string.back()))
          {
            token.token_type = TokenType::INTEGER_LITERAL;
            // TODO Now we must assert that all internal parts are numbers as well...
          }
          else
          {
            if (token.string.back() == 's')
              token.token_type = TokenType::PRIMITIVE_SIGNED;
            else if (token.string.back() == 'u')
              token.token_type = TokenType::PRIMITIVE_UNSIGNED;
          }
        }
      }
      else
      {
        token.token_type = TokenType::UNIDENTIFIED;
      }
    }

    protocols::Type Lexer::typify (char val)
    {
      using namespace protocols;
      if (65 <= val && val <= 90 || 97 <= val && val <= 122 || val == 95 || 48 <= val && val <= 57)
        return Type::alphanumu;
      if (val == '"')
        return Type::quote;
      if (isAnyOf(val, '(', ')', '{', '}', '[', ']'))
        return Type::grouper;
      else if (isAnyOf(val, ' ', '\n', '\r', '\f', '\v', '\t'))
        return Type::whitespace;
      else
        return Type::symbol;
    }
  } // namespace lexer
} // namespace tul
