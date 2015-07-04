#include "Parser.hpp"
#include "dependency/TokenTypeToCrossTerminal.hpp"


namespace tul
{
  namespace parser
  {

    Parser::Parser()
    {
      using namespace protocols;
      symbol_stack.emplace(new ConcreteSyntaxTree);
      symbol_stack.top()->node_type = CrossTerminal::ENTER;
    }

    protocols::CrossTerminal Parser::convertTokenTypeToCrossTerminal(const protocols::TokenType token_type)
    {
      using namespace protocols;
      #define cT(name) CrossTerminal::name
        switch (token_type)
        {
          default: return cT(NONE);
        }
      #undef cT
    }

    /**
      Here we simply decode the token's symbol into a CrossTerminal
      and feed it to the CrossTerminalParser. This will be a big switch for
      converting the TokenType into a CrossTerminal, in addition to keeping
      a stack containing all bottom nodes of the production tree.

      The CrossTerminalParser will return a vector of new children and an
      action. The CrossTerminalParser's job is to set up the parsing
      table.
    */
    bool Parser::parseSymbol(const protocols::Token &input_token)
    {
      protocols::CrossTerminal cross_terminal = dependency::TokenTypeToCrossTerminal::convertToCrossTerminal(input_token.token_type);

      return false;
    }

  }
}
