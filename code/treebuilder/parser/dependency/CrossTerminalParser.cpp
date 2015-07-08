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
#include "CrossTerminalParser.hpp"


namespace tul
{
  namespace treebuilder
  {
    namespace parser
    {
      namespace dependency
      {
        CrossTerminalParser::CrossTerminalParser()
        {
          // Set up parse table
          using namespace protocols;
          #define cT(name) CrossTerminal::name
          #define eps(left) ll_parser.addEpsilon(CrossTerminal::left)
          #define add(left, middle) \
            if (ll_parser.doesRuleExist(CrossTerminal::left, CrossTerminal::middle)) throw std::string(#left) + " already has transition " + std::string(#middle); \
            ll_parser.addRule(CrossTerminal::left, CrossTerminal::middle,

            // We start with simple `private 32u x = 3; public Something y(arg: "hello!");`
            add(ENTER, KEYWORD_PRIVATE) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_ASSIGNMENT), cT(SYMBOL_SEMICOLON), cT(FIELD_DECLARATION_LIST)});
            add(ENTER, KEYWORD_PUBLIC) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_ASSIGNMENT), cT(SYMBOL_SEMICOLON), cT(FIELD_DECLARATION_LIST)});
            add(ENTER, KEYWORD_RESTRICTED) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_ASSIGNMENT), cT(SYMBOL_SEMICOLON), cT(FIELD_DECLARATION_LIST)});

            add(FIELD_DECLARATION_LIST, KEYWORD_PRIVATE) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_ASSIGNMENT), cT(SYMBOL_SEMICOLON), cT(FIELD_DECLARATION_LIST)});
            add(FIELD_DECLARATION_LIST, KEYWORD_PUBLIC) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_ASSIGNMENT), cT(SYMBOL_SEMICOLON), cT(FIELD_DECLARATION_LIST)});
            add(FIELD_DECLARATION_LIST, KEYWORD_RESTRICTED) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_ASSIGNMENT), cT(SYMBOL_SEMICOLON), cT(FIELD_DECLARATION_LIST)});

            // Used in declarations: `private 32u x = 3; public Something y(arg: "hello!");`, the `= 3` and `(arg: "hello!")`
            add(OPTIONAL_ASSIGNMENT, GROUPER_LEFT_PARENTHESIS) {cT(GROUPER_LEFT_PARENTHESIS), cT(ARGUMENT_LIST), cT(GROUPER_RIGHT_PARENTHESIS)});
            add(OPTIONAL_ASSIGNMENT, SYMBOL_EQUAL) {cT(SYMBOL_EQUAL), cT(EXPRESSION)});

            eps(DECLARATION);
            eps(OPTIONAL_EQUALS);
            eps(OPTIONAL_ASSIGNMENT);
            eps(FIELD_DECLARATION_LIST);
            eps(OPTIONAL_ASSIGNMENT);

            add(ACCESS_SPECIFIER, KEYWORD_PRIVATE) {cT(KEYWORD_PRIVATE)});
            add(ACCESS_SPECIFIER, KEYWORD_PUBLIC) {cT(KEYWORD_PUBLIC)});
            add(ACCESS_SPECIFIER, KEYWORD_RESTRICTED) {cT(KEYWORD_RESTRICTED)});

            add(DECLARATION, IDENTIFIER_CLASS) {cT(IDENTIFIER_CLASS), cT(IDENTIFIER_VARIABLE)});
            add(DECLARATION, PRIMITIVE_SIGNED) {cT(PRIMITIVE_SIGNED), cT(IDENTIFIER_VARIABLE)});
            add(DECLARATION, PRIMITIVE_UNSIGNED) {cT(PRIMITIVE_UNSIGNED), cT(IDENTIFIER_VARIABLE)});

            add(EXPRESSION, INTEGER_LITERAL) {cT(INTEGER_LITERAL)});
            add(EXPRESSION, STRING) {cT(STRING), cT(OPTIONAL_STRING_PLUS)});

            add(OPTIONAL_STRING_PLUS, SYMBOL_PLUS) {cT(SYMBOL_PLUS), cT(STRING), cT(OPTIONAL_STRING_PLUS)});
            eps(OPTIONAL_STRING_PLUS);

          #undef add
          #undef eps
          #undef cT
        }

        std::vector<protocols::CrossTerminal> CrossTerminalParser::calculateExpectedTokens(const protocols::CrossTerminal stack_top)
        {
          return ll_parser.getTransitionTokens(stack_top);
        }

        protocols::ParseReturn<protocols::CrossTerminal> CrossTerminalParser::parseSymbol
        (const protocols::CrossTerminal stack_top, const protocols::CrossTerminal &input_element)
        {
          using namespace protocols;
          return ll_parser.parseSymbol(input_element, stack_top);
        }
      }
    }
  }
}
