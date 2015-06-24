#include "TokenGenerator.hpp"
#include "protocols/Action.hpp"
#include "protocols/Token.hpp"

namespace tul
{
  namespace lexer
  {
    namespace dependency
    {

      bool TokenGenerator::consumeCharacter(char character, protocols::Action action)
      {
        using namespace protocols;
        switch (action)
        {
          case Action::n:
            // Do nothing
            return false;
          break;
          case Action::p:
            current_working_lexeme.push_back(character);
            return false;
          break;
          case Action::ptg:
            current_working_lexeme.push_back(character);
            tokens.emplace_back(Token {0, Type::grouper, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          case Action::taptg:
            tokens.emplace_back(Token {0, Type::alphanumu, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
            current_working_lexeme.push_back(character);
            tokens.emplace_back(Token {0, Type::grouper, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          case Action::ta:
            tokens.emplace_back(Token {0, Type::alphanumu, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          case Action::tap:
            tokens.emplace_back(Token {0, Type::alphanumu, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
            current_working_lexeme.push_back(character);
          break;
          case Action::trp:
            tokens.emplace_back(Token {0, Type::quote, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
            current_working_lexeme.push_back(character);
          break;
          case Action::trptg:
            tokens.emplace_back(Token {0, Type::quote, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
            current_working_lexeme.push_back(character);
            tokens.emplace_back(Token {0, Type::grouper, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          case Action::tsp:
            tokens.emplace_back(Token {0, Type::symbol, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
            current_working_lexeme.push_back(character);
          break;
          case Action::tsptg:
            tokens.emplace_back(Token {0, Type::symbol, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
            current_working_lexeme.push_back(character);
            tokens.emplace_back(Token {0, Type::grouper, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          case Action::ts:
            tokens.emplace_back(Token {0, Type::symbol, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          case Action::tr:
            tokens.emplace_back(Token {0, Type::quote, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          default:
          break;
        }
        return true;
      }

      std::vector<protocols::Token> &TokenGenerator::getTokenStack()
      {
        return tokens;
      }
    } // namespace dependency
  } // namespace lexer
} // namespace tul
