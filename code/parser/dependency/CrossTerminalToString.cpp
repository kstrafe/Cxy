#include "CrossTerminalToString.hpp"


namespace tul
{
  namespace parser
  {
    namespace dependency
    {
      std::string CrossTerminalToString::convertToString(protocols::CrossTerminal cross_terminal)
      {
        using namespace protocols;
        #define gen(name) case protocols::CrossTerminal::name: return #name
        #define ben(name, string) case protocols::CrossTerminal::name: return string
          switch (cross_terminal)
          {
            gen(ENTER);
            gen(EXIT);

            gen(NONE);

            gen(FIELD_DECLARATION_LIST);
            gen(DECLARATION);
            gen(ACCESS_SPECIFIER);
            gen(OPTIONAL_EQUALS);
            gen(COMMA_SEPARATED_LIST);
            gen(OPTIONAL_CALL);
            gen(METHOD_DECLARATION_LIST);
            gen(COMMA_SEPARATED_TYPE_AND_NAME_LIST);
            gen(STATEMENT_LIST);

            gen(COMMA_SEPARATED_LIST_ITEM);
            gen(STATEMENT);
            gen(TYPE);

            gen(GROUPER_LEFT_BRACE);
            gen(GROUPER_RIGHT_BRACE);
            gen(GROUPER_LEFT_BRACKET);
            gen(GROUPER_RIGHT_BRACKET);
            gen(GROUPER_LEFT_PARENTHESIS);
            gen(GROUPER_RIGHT_PARENTHESIS);

            gen(IDENTIFIER_CLASS);
            gen(IDENTIFIER_ENUMERATION);
            gen(IDENTIFIER_PACKAGE);
            gen(IDENTIFIER_SUBROUTINE);
            gen(IDENTIFIER_VARIABLE);

            gen(INTEGER_LITERAL);

            gen(KEYWORD_ASSEMBLY);
            gen(KEYWORD_DO);
            gen(KEYWORD_FOR);
            gen(KEYWORD_GOTO);
            gen(KEYWORD_IF);
            gen(KEYWORD_LABEL);
            gen(KEYWORD_PRIVATE);
            gen(KEYWORD_PUBLIC);
            gen(KEYWORD_RESTRICTED);
            gen(KEYWORD_WHILE);

            gen(PRIMITIVE_SIGNED);
            gen(PRIMITIVE_UNSIGNED);

            gen(STRING);

            gen(SYMBOL_PLUS);
            gen(SYMBOL_PLUS_PLUS);
            gen(SYMBOL_MINUS);
            gen(SYMBOL_MINUS_MINUS);
            gen(SYMBOL_STAR);
            gen(SYMBOL_STAR_STAR);
            gen(SYMBOL_FORWARD_SLASH);
            gen(SYMBOL_BACKWARD_SLASH);
            gen(SYMBOL_AMPERSAND_AMPERSAND);
            gen(SYMBOL_BAR_BAR);
            gen(SYMBOL_AMPERSAND);
            gen(SYMBOL_BAR);
            gen(SYMBOL_CARET);
            gen(SYMBOL_EXCLAMATION_MARK);
            gen(SYMBOL_DOT_DOT);
            gen(SYMBOL_DOT);
            gen(SYMBOL_COMMA);
            gen(SYMBOL_COLON_COLON);
            gen(SYMBOL_COLON);
            gen(SYMBOL_SEMICOLON);
            gen(SYMBOL_LESS_THAN_LESS_THAN);
            gen(SYMBOL_LESS_THAN);
            gen(SYMBOL_GREATER_THAN_GREATER_THAN);
            gen(SYMBOL_GREATER_THAN);
            default: return "";
          }
        #undef ben
        #undef gen
      }
    }
  }
}
