#pragma once

#include "dependency/CrossTerminalParser.hpp"

#include "protocols/ConcreteSyntaxTree.hpp"
#include "protocols/CrossTerminal.hpp"
#include "protocols/Token.hpp"

#include <stack>

namespace tul
{
  namespace parser
  {
    /**
      Implement a simple LL(1) parser for our language. This is a mathematical model of
      a parser. It runs in O(n) time. The protocols used will be the Token and ConcreteSyntaxTree.

      The parser will be table/state based as it has to be re-entrant. The mealy machine can be of
      use here.

      The parser simply looks up the current lookahead and the top token on the stack. We can
      idealize the top token on the stack as the state.

      Entry [state][lookahead] will give us a result from the mealy machine. For example, such a
      result will be the action "transfer with '(', 'boolean_expression', ')'". This will put those
      tokens on top of the stack. It will also need to link the last token into a tree somehow.

      transfer[top][input] => {New, Terminals, And, NonjjjjjjTerminals}
    */
    class Parser
    {
    public:

      Parser();
      bool parseSymbol(const protocols::Token &input_token);

    private:

      dependency::CrossTerminalParser cross_parser;
      protocols::ConcreteSyntaxTree *syntax_tree_root;
      std::stack<protocols::ConcreteSyntaxTree *> symbol_stack;

    };
  }
}
