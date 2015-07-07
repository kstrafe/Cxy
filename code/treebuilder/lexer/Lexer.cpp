// Copyright © 2015 Kevin Robert Stravers
/*
This file is part of Unnamed-Language Compiler Reference Implementation (ULCRI).

ULCRI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ULCRI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ULCRI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Lexer.hpp"
#include "protocols/Token.hpp"
#include "dependency/KeywordMatcher.hpp"
#include "dependency/SymbolMatcher.hpp"

#include <algorithm>
#include <limits>
#include <vector>

namespace tul
{
  namespace lexer
  {
    bool Lexer::insertCharacter(char character_)
    {
      protocols::EntryType type_of_character = typify(character_);
      protocols::Action action_to_perform = action_generator.computeAction(type_of_character);
      {
        std::size_t amount_of_new_tokens = token_generator.consumeCharacter(character_, action_to_perform);
        if (amount_of_new_tokens == std::numeric_limits<std::size_t>::max())
        {
          return false;
        }
        // A new token is ready to be identified
        for (std::size_t new_token = 0; new_token < amount_of_new_tokens; ++new_token)
        {
          identifyToken(getTokenStack()[getTokenStack().size() - 1 - new_token]);
          getTokenStack()[getTokenStack().size() - 1 - new_token].column_number = position_counter.getCurrentColumnNumber();
          getTokenStack()[getTokenStack().size() - 1 - new_token].line_number = position_counter.getCurrentLineNumber();
        }
      }
      position_counter.countCharacter(character_);
      return true;
    }

    std::vector<protocols::Token> &Lexer::getTokenStack()
    {
      return token_generator.getTokenStack();
    }

    protocols::TokenType Lexer::getKeyword(const std::string &test_lexeme) const
    {
      return dependency::KeywordMatcher::getKeyword(test_lexeme);
    }

    void Lexer::identifyToken(protocols::Token &input_token)
    {
      using namespace protocols;

      auto all_digit = [&input_token]() -> bool {return std::all_of(input_token.accompanying_lexeme.cbegin(), input_token.accompanying_lexeme.cend(), [](char in_character) -> bool {return std::isdigit(in_character);});};
      auto all_lower = [&input_token]() -> bool {return std::all_of(input_token.accompanying_lexeme.cbegin(), input_token.accompanying_lexeme.cend(), [](char in_character) -> bool {return std::islower(in_character);});};
      auto all_upper = [&input_token]() -> bool {return std::all_of(input_token.accompanying_lexeme.cbegin(), input_token.accompanying_lexeme.cend(), [](char in_character) -> bool {return std::isupper(in_character);});};

      auto any_lower = [&input_token]() -> bool {return std::any_of(input_token.accompanying_lexeme.cbegin(), input_token.accompanying_lexeme.cend(), [](char in_character) -> bool {return std::islower(in_character);});};
      auto any_underscore = [&input_token]() -> bool {return std::any_of(input_token.accompanying_lexeme.cbegin(), input_token.accompanying_lexeme.cend(), [](char in_character) -> bool {return in_character == '_'; });};
      auto any_upper = [&input_token]() -> bool {return std::any_of(input_token.accompanying_lexeme.cbegin(), input_token.accompanying_lexeme.cend(), [](char in_character) -> bool {return std::isupper(in_character);});};

      auto begins_with_lowercase = [&input_token]() -> bool {return std::islower(input_token.accompanying_lexeme.front());};
      auto begins_with_uppercase = [&input_token]() -> bool {return std::isupper(input_token.accompanying_lexeme.front());};

      auto ends_with_lowercase = [&input_token]() -> bool {return std::islower(input_token.accompanying_lexeme.back());};

      auto is_class_identifier = [&]() -> bool {return begins_with_uppercase() && any_underscore() == false && ends_with_lowercase();};
      auto is_enumeration_identifier = [&]() -> bool {return (begins_with_uppercase() && any_underscore() == true && any_lower() == false) || all_upper();};
      auto is_function_identifier = [&]() -> bool {return begins_with_lowercase() && any_underscore() == false && ends_with_lowercase() && any_upper();};
      auto is_keyword = [&]() -> bool {return getKeyword(input_token.accompanying_lexeme) != TokenType::UNIDENTIFIED;};
      auto is_number_literal = [&]() -> bool {return all_digit();};
      auto is_package_identifier = [&]() -> bool {return all_lower();};
      auto is_primitive_signed = [&]() -> bool {return input_token.accompanying_lexeme.back() == 's' && std::all_of(input_token.accompanying_lexeme.cbegin(), input_token.accompanying_lexeme.cend() - 1, [](char in_character) -> bool {return std::isdigit(in_character);});};
      auto is_primitive_unsigned = [&]() -> bool {return input_token.accompanying_lexeme.back() == 'u' && std::all_of(input_token.accompanying_lexeme.cbegin(), input_token.accompanying_lexeme.cend() - 1, [](char in_character) -> bool {return std::isdigit(in_character);});};
      auto is_variable_identifier = [&]() -> bool {return any_underscore() && any_upper() == false && any_lower();};

      /*
        ClassNamesAreLikeThisNoUnderScoreEndWithSmallCharacter
        variable_names_are_like_this_must_have_underscore, _cool
        functionNamesMustAlwaysHaveAtLeastOneSmallBeginningCharacter
        ENUMERATIVENAMESLIKETHIS, THEY_ALLOW_UNDERSCORES
        packagenamesarefullylowercasewithoutunderscore
      */

      if (input_token.entry_type == EntryType::ALPHA_DIGIT_OR_UNDERSCORE)
      {
        if (is_class_identifier())
        {
           input_token.token_type = TokenType::IDENTIFIER_CLASS;
        }
        else if (is_enumeration_identifier())
        {
          input_token.token_type = TokenType::IDENTIFIER_ENUMERATION;
        }
        else if (is_function_identifier())
        {
          input_token.token_type = TokenType::IDENTIFIER_SUBROUTINE;
        }
        else if (is_number_literal())
        {
          input_token.token_type = TokenType::INTEGER_LITERAL;
        }
        else if (is_keyword())
        {
          input_token.token_type = getKeyword(input_token.accompanying_lexeme);
        }
        else if (is_package_identifier())
        {
          input_token.token_type = TokenType::IDENTIFIER_PACKAGE;
        }
        else if (is_primitive_signed())
        {
          input_token.token_type = TokenType::PRIMITIVE_SIGNED;
        }
        else if (is_primitive_unsigned())
        {
          input_token.token_type = TokenType::PRIMITIVE_UNSIGNED;
        }
        else if (is_variable_identifier())
        {
          input_token.token_type = TokenType::IDENTIFIER_VARIABLE;
        }
        else
        {
          input_token.token_type = TokenType::UNIDENTIFIED;
        }
      }
      else if (input_token.entry_type == EntryType::GROUPING_SYMBOL)
      {
        switch (input_token.accompanying_lexeme.at(0))
        #define caze(enum_token_type) input_token.token_type = TokenType::enum_token_type; break;
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
      else if (input_token.entry_type == EntryType::QUOTE_SYMBOL)
      {
        input_token.token_type = TokenType::STRING;
      }
      else if (input_token.entry_type == EntryType::OTHER_SYMBOL)
      {
        input_token.token_type = dependency::SymbolMatcher::getSymbol(input_token.accompanying_lexeme);
      }
      else
      {
        input_token.token_type = TokenType::UNIDENTIFIED;
      }
    }

    protocols::EntryType Lexer::typify (char val_)
    {
      using namespace protocols;
      const constexpr unsigned char UTF_8_LIMIT = 128;
      if ((65 <= val_ && val_ <= 90) || (97 <= val_ && val_ <= 122) || val_ == 95 || (48 <= val_ && val_ <= 57))
        return EntryType::ALPHA_DIGIT_OR_UNDERSCORE;
      if (val_ == '"')
        return EntryType::QUOTE_SYMBOL;
      if (isAnyOf(val_, '(', ')', '{', '}', '[', ']'))
        return EntryType::GROUPING_SYMBOL;
      else if (isAnyOf(val_, ' ', '\n', '\r', '\f', '\v', '\t'))
        return EntryType::WHITESPACE;
      else if (static_cast<unsigned char>(val_) >= UTF_8_LIMIT || isAnyOf(val_, 0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 127))
        return EntryType::UNKNOWN_CODE_POINT;
      else
        return EntryType::OTHER_SYMBOL;
    }
  } // namespace lexer
} // namespace tul
