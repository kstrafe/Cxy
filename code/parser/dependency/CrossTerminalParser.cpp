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

          /*
            Note that we put the non-terminals in a greedy, alphabetic order.
            ENTER derives MODULE_DATA_LIST and MODULE_METHOD_LIST in that order,
            this means that the next two entries are those non-terminals
            in that specific order. The reason is to keep the grammar
            consistent and easy to read. Transitive transitions are taken
            _after_ having derived the previous collection.
            All transitions from the previous one have zero lines between themselves.
            From this we derive terminals and non-terminals
          */
          // We start with simple `private 32u x = 3; public Something y(arg: "hello!");`
          add(ENTER, KEYWORD_PRIVATE) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_ASSIGNMENT), cT(SYMBOL_SEMICOLON), cT(FIELD_DECLARATION_LIST)});
          add(ENTER, KEYWORD_PUBLIC) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_ASSIGNMENT), cT(SYMBOL_SEMICOLON), cT(FIELD_DECLARATION_LIST)});
          add(ENTER, KEYWORD_RESTRICTED) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_ASSIGNMENT), cT(SYMBOL_SEMICOLON), cT(FIELD_DECLARATION_LIST)});

          add(FIELD_DECLARATION_LIST, KEYWORD_PRIVATE) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_ASSIGNMENT), cT(SYMBOL_SEMICOLON), cT(FIELD_DECLARATION_LIST)});
          add(FIELD_DECLARATION_LIST, KEYWORD_PUBLIC) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_ASSIGNMENT), cT(SYMBOL_SEMICOLON), cT(FIELD_DECLARATION_LIST)});
          add(FIELD_DECLARATION_LIST, KEYWORD_RESTRICTED) {cT(ACCESS_SPECIFIER), cT(DECLARATION), cT(OPTIONAL_ASSIGNMENT), cT(SYMBOL_SEMICOLON), cT(FIELD_DECLARATION_LIST)});

          // Used in declarations: `private 32u x = 3; public Something y(arg: "hello!");`, the `= 3` and `(arg: "hello!")`
          add(OPTIONAL_ASSIGNMENT, GROUPER_LEFT_PARENTHESIS) {cT(GROUPER_LEFT_PARENTHESIS), cT(ARGUMENT_LIST), cT(GROUPER_RIGHT_PARENTHESIS)});
          add(OPTIONAL_ASSIGNMENT, SYMBOL_EQUAL) {cT(EXPRESSION)});

          eps(DECLARATION);
          eps(OPTIONAL_EQUALS);
          eps(FIELD_DECLARATION_LIST);
          eps(OPTIONAL_ASSIGNMENT);

          add(ACCESS_SPECIFIER, KEYWORD_PRIVATE) {cT(KEYWORD_PRIVATE)});
          add(ACCESS_SPECIFIER, KEYWORD_PUBLIC) {cT(KEYWORD_PUBLIC)});
          add(ACCESS_SPECIFIER, KEYWORD_RESTRICTED) {cT(KEYWORD_RESTRICTED)});

          add(DECLARATION, IDENTIFIER_CLASS) {cT(IDENTIFIER_CLASS), cT(IDENTIFIER_VARIABLE)});

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
