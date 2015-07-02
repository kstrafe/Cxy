#include "CrossTerminalParser.hpp"


namespace tul
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

          add(ENTER, KEYWORD_PRIVATE) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_EQUALS), cT(FIELD_DECLARATION_LIST)});
          add(ENTER, KEYWORD_PUBLIC) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_EQUALS), cT(FIELD_DECLARATION_LIST)});
          add(ENTER, KEYWORD_RESTRICTED) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_EQUALS), cT(FIELD_DECLARATION_LIST)});
          add(FIELD_DECLARATION_LIST, KEYWORD_PRIVATE) {cT(DECLARATION), cT(OPTIONAL_EQUALS), cT(FIELD_DECLARATION_LIST)});
          add(FIELD_DECLARATION_LIST, KEYWORD_PUBLIC) {cT(DECLARATION), cT(OPTIONAL_EQUALS), cT(FIELD_DECLARATION_LIST)});
          add(FIELD_DECLARATION_LIST, KEYWORD_RESTRICTED) {cT(DECLARATION), cT(OPTIONAL_EQUALS), cT(FIELD_DECLARATION_LIST)});

          eps(DECLARATION);
          eps(OPTIONAL_EQUALS);
          eps(FIELD_DECLARATION_LIST);

          add(ACCESS_SPECIFIER, KEYWORD_PRIVATE) {cT(KEYWORD_PRIVATE)});
          add(ACCESS_SPECIFIER, KEYWORD_PUBLIC) {cT(KEYWORD_PUBLIC)});
          add(ACCESS_SPECIFIER, KEYWORD_RESTRICTED) {cT(KEYWORD_RESTRICTED)});

          add(DECLARATION, IDENTIFIER_CLASS) {cT(IDENTIFIER_CLASS), cT(IDENTIFIER_VARIABLE)});

        #undef add
        #undef eps
        #undef cT
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
