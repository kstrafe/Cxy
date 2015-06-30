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
        #define eps(left) ll_parser.addEpsilon(left)
        #define add(left, middle) \
          if (ll_parser.doesRuleExist(left, middle)) throw std::string(#left) + " already has transition " + std::string(#middle); \
          ll_parser.addRule(left, middle,

          add(CrossTerminal::NONE, CrossTerminal::ENTER) {CrossTerminal::ENTER});
          eps(CrossTerminal::ENTER);
        #undef add
      }

      protocols::ParseReturn<protocols::CrossTerminal> CrossTerminalParser::parseSymbol(const protocols::CrossTerminal &input_element)
      {
        using namespace protocols;
        return {nullptr, ParseReturn<CrossTerminal>::Action::OBSERVE_ERROR};
      }
    }
  }
}
