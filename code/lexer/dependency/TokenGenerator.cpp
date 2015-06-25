#include "TokenGenerator.hpp"
#include "protocols/Action.hpp"
#include "protocols/Token.hpp"
#include "protocols/EntryType.hpp"

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
          case Action::N:
            // Do nothing
            return false;
          break;
          case Action::P:
            current_working_lexeme.push_back(character);
            return false;
          break;
          case Action::PTG:
            current_working_lexeme.push_back(character);
            tokens.emplace_back(Token {0, EntryType::GROUPING_SYMBOL, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          case Action::TAPTG:
            tokens.emplace_back(Token {0, EntryType::ALPHA_DIGIT_OR_UNDERSCORE, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
            current_working_lexeme.push_back(character);
            tokens.emplace_back(Token {0, EntryType::GROUPING_SYMBOL, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          case Action::TA:
            tokens.emplace_back(Token {0, EntryType::ALPHA_DIGIT_OR_UNDERSCORE, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          case Action::TAP:
            tokens.emplace_back(Token {0, EntryType::ALPHA_DIGIT_OR_UNDERSCORE, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
            current_working_lexeme.push_back(character);
          break;
          case Action::TRP:
            tokens.emplace_back(Token {0, EntryType::QUOTE_SYMBOL, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
            current_working_lexeme.push_back(character);
          break;
          case Action::TRPTG:
            tokens.emplace_back(Token {0, EntryType::QUOTE_SYMBOL, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
            current_working_lexeme.push_back(character);
            tokens.emplace_back(Token {0, EntryType::GROUPING_SYMBOL, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          case Action::TSP:
            tokens.emplace_back(Token {0, EntryType::OTHER_SYMBOL, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
            current_working_lexeme.push_back(character);
          break;
          case Action::TSPTG:
            tokens.emplace_back(Token {0, EntryType::OTHER_SYMBOL, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
            current_working_lexeme.push_back(character);
            tokens.emplace_back(Token {0, EntryType::GROUPING_SYMBOL, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          case Action::TS:
            tokens.emplace_back(Token {0, EntryType::OTHER_SYMBOL, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
          break;
          case Action::TR:
            tokens.emplace_back(Token {0, EntryType::QUOTE_SYMBOL, TokenType::UNIDENTIFIED, std::move(current_working_lexeme)} );
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
