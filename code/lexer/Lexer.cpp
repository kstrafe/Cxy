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
      protocols::EntryType type_of_character = typify(character);
      protocols::Action action_to_perform = action_generator.computeAction(type_of_character);
      {
        std::size_t amount = token_generator.consumeCharacter(character, action_to_perform);
        // A new token is ready to be identified
        for (std::size_t element = 0; element < amount; ++element)
        {
          identifyToken(getTokenStack()[getTokenStack().size() - 1 - element]);
        }
      }
    }

    std::vector<protocols::Token> &Lexer::getTokenStack()
    {
      return token_generator.getTokenStack();
    }

    void Lexer::identifyToken(protocols::Token &token)
    {
      using namespace protocols;

      auto all_digit = [&token]() -> bool {return std::all_of(token.string.cbegin(), token.string.cend(), [](char character) -> bool {return std::isdigit(character);});};
      auto all_lower = [&token]() -> bool {return std::all_of(token.string.cbegin(), token.string.cend(), [](char character) -> bool {return std::islower(character);});};
      auto all_upper = [&token]() -> bool {return std::all_of(token.string.cbegin(), token.string.cend(), [](char character) -> bool {return std::isupper(character);});};

      auto any_lower = [&token]() -> bool {return std::any_of(token.string.cbegin(), token.string.cend(), [](char character) -> bool {return std::islower(character);});};
      auto any_underscore = [&token]() -> bool {return std::any_of(token.string.cbegin(), token.string.cend(), [](char character) -> bool {return character == '_'; });};
      auto any_upper = [&token]() -> bool {return std::any_of(token.string.cbegin(), token.string.cend(), [](char character) -> bool {return std::isupper(character);});};

      auto begins_with_lowercase = [&token]() -> bool {return std::islower(token.string.front());};
      auto begins_with_underscore = [&token]() -> bool {return token.string.front() == '_';};
      auto begins_with_uppercase = [&token]() -> bool {return std::isupper(token.string.front());};

      auto ends_with_lowercase = [&token]() -> bool {return std::islower(token.string.back());};
      auto ends_with_lowercase_u_or_s = [&token]() -> bool {return token.string.back() == 'u' || token.string.back() == 's';};

      auto is_class_identifier = [&]() -> bool {return begins_with_uppercase() && any_underscore() == false && ends_with_lowercase();};
      auto is_enumeration_identifier = [&]() -> bool {return begins_with_uppercase() && any_underscore() == true && any_lower() == false || all_upper();};
      auto is_function_identifier = [&]() -> bool {return begins_with_lowercase() && any_underscore() == false && ends_with_lowercase() && any_upper();};
      auto is_number_literal = [&]() -> bool {return all_digit();};
      auto is_package_identifier = [&]() -> bool {return all_lower();};
      auto is_primitive_signed = [&]() -> bool {return token.string.back() == 's' && std::all_of(token.string.cbegin(), token.string.cend() - 1, [](char character) -> bool {return std::isdigit(character);});};
      auto is_primitive_unsigned = [&]() -> bool {return token.string.back() == 'u' && std::all_of(token.string.cbegin(), token.string.cend() - 1, [](char character) -> bool {return std::isdigit(character);});};
      auto is_variable_identifier = [&]() -> bool {return any_underscore() && any_upper() == false && any_lower();};

      // enum class EntryType { ALPHA_DIGIT_OR_UNDERSCORE, GROUPING_SYMBOL, QUOTE_SYMBOL, OTHER_SYMBOL, WHITESPACE };
      // Bingo, class name. We'll need to assert that there are:
      // No underscores
      // At least one lowercase character?
      // Say:
      /*
        ClassNamesAreLikeThisNoUnderScoreEndWithSmallCharacter
        variable_names_are_like_this_must_have_underscore, _cool
        functionNamesMustAlwaysHaveAtLeastOneSmallBeginningCharacter
        ENUMERATIVENAMESLIKETHIS, THEY_ALLOW_UNDERSCORES
        packagenamesarefullylowercasewithoutunderscore
      */

      if (token.entry_type == EntryType::ALPHA_DIGIT_OR_UNDERSCORE)
      {
        if (is_class_identifier())
        {
           token.token_type = TokenType::IDENTIFIER_CLASS;
        }
        else if (is_enumeration_identifier())
        {
          token.token_type = TokenType::IDENTIFIER_ENUMERATION;
        }
        else if (is_function_identifier())
        {
          token.token_type = TokenType::IDENTIFIER_SUBROUTINE;
        }
        else if (is_number_literal())
        {
          token.token_type = TokenType::INTEGER_LITERAL;
        }
        else if (is_package_identifier())
        {
          token.token_type = TokenType::IDENTIFIER_PACKAGE;
        }
        else if (is_primitive_signed())
        {
          token.token_type = TokenType::PRIMITIVE_SIGNED;
        }
        else if (is_primitive_unsigned())
        {
          token.token_type = TokenType::PRIMITIVE_UNSIGNED;
        }
        else if (is_variable_identifier())
        {
          token.token_type = TokenType::IDENTIFIER_VARIABLE;
        }
      }
      else if (token.entry_type == EntryType::GROUPING_SYMBOL)
      {
        switch (token.string.at(0))
        #define caze(x) token.token_type = TokenType::x; break;
        {
          case '(': caze(GROUPER_LEFT_PARENTHESIS)
          case ')': caze(GROUPER_RIGHT_PARENTHESIS)
          case '[': caze(GROUPER_LEFT_BRACKET)
          case ']': caze(GROUPER_RIGHT_BRACKET)
          case '{': caze(GROUPER_LEFT_BRACE)
          case '}': caze(GROUPER_RIGHT_BRACE)
          default: break;
        }
        #undef caze
      }
      else if (token.entry_type == EntryType::OTHER_SYMBOL)
      {
        #define caze(string_, symbol) if (token.string == string_) { token.token_type = TokenType::symbol; }
          caze("+", SYMBOL_PLUS)
          else caze("++", SYMBOL_PLUS_PLUS)
          else caze("-", SYMBOL_MINUS)
          else caze("--", SYMBOL_MINUS_MINUS)
          else caze("*", SYMBOL_STAR)
          else caze("*", SYMBOL_STAR_STAR)
          else caze("/", SYMBOL_FORWARD_SLASH)
          else caze("&&", SYMBOL_AMPERSAND_AMPERSAND)
          else caze("||", SYMBOL_BAR_BAR)
          else caze("&", SYMBOL_AMPERSAND)
          else caze("|", SYMBOL_BAR)
          else caze("^", SYMBOL_CARET)
          else caze("!", SYMBOL_EXCLAMATION_MARK)
          else caze("..", SYMBOL_DOT_DOT)
          else caze(".", SYMBOL_DOT)
          else caze(",", SYMBOL_COMMA)
          else caze("::", SYMBOL_COLON_COLON)
          else caze(":", SYMBOL_COLON)
          else caze(";", SYMBOL_SEMICOLON)
          else caze("<<", SYMBOL_LESS_THAN_LESS_THAN)
          else caze("<", SYMBOL_LESS_THAN_LESS_THAN)
          else caze(">>", SYMBOL_GREATER_THAN_GREATER_THAN)
          else caze(">", SYMBOL_GREATER_THAN)
          else caze("\\", SYMBOL_BACKWARD_SLASH)
        #undef caze
      }
      else
      {
        token.token_type = TokenType::UNIDENTIFIED;
      }
    }

    protocols::EntryType Lexer::typify (char val)
    {
      using namespace protocols;
      if (65 <= val && val <= 90 || 97 <= val && val <= 122 || val == 95 || 48 <= val && val <= 57)
        return EntryType::ALPHA_DIGIT_OR_UNDERSCORE;
      if (val == '"')
        return EntryType::QUOTE_SYMBOL;
      if (isAnyOf(val, '(', ')', '{', '}', '[', ']'))
        return EntryType::GROUPING_SYMBOL;
      else if (isAnyOf(val, ' ', '\n', '\r', '\f', '\v', '\t'))
        return EntryType::WHITESPACE;
      else
        return EntryType::OTHER_SYMBOL;
    }
  } // namespace lexer
} // namespace tul
