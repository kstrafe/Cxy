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

          add(ENTER, GROUPER_LEFT_PARENTHESIS) {cT(ENTER)});
          add(ENTER, KEYWORD_PRIVATE) {cT(DECLARATION), cT(OPTIONAL_EQUALS)});
        #undef add
        #undef eps
        #undef cT
      }

      protocols::ParseReturn<protocols::CrossTerminal> CrossTerminalParser::parseSymbol(const protocols::CrossTerminal &input_element)
      {
        using namespace protocols;
        return {nullptr, ParseReturn<CrossTerminal>::Action::OBSERVE_ERROR};
      }
    }
  }
}
