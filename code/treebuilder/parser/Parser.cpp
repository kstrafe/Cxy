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
#include "Parser.hpp"
#include "dependency/TokenTypeToCrossTerminal.hpp"


namespace tul
{
  namespace parser
  {

    Parser::Parser()
    {
      using namespace protocols;
      symbol_stack.emplace(syntax_tree_root = new ConcreteSyntaxTree);
      symbol_stack.top()->node_type = CrossTerminal::ENTER;
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
      using namespace protocols;
      CrossTerminal cross_terminal = dependency::TokenTypeToCrossTerminal::convertToCrossTerminal(input_token.token_type);
      while (symbol_stack.empty() == false)
      {
        ParseReturn<protocols::CrossTerminal> parse_return = cross_parser.parseSymbol(symbol_stack.top()->node_type, cross_terminal);
        switch (parse_return.desired_action)
        {
          case ParseReturn<CrossTerminal>::Action::REMOVE_TOP: // Match, also remove from the input stack, by returning, we imply this.
            symbol_stack.top()->token_ = input_token;
            symbol_stack.top()->node_type = cross_terminal;
            symbol_stack.pop();
            return true;
          case ParseReturn<CrossTerminal>::Action::EPSILONATE:
            symbol_stack.top()->node_type = CrossTerminal::EPSILONATE;
            symbol_stack.pop();
            continue;
          case ParseReturn<CrossTerminal>::Action::CONTINUE:
          {
            ConcreteSyntaxTree *tree_ptr = symbol_stack.top();
            symbol_stack.pop();
            for (std::size_t i_ = parse_return.child_symbols->size() - 1; i_ >= 0; --i_)
            {
              ConcreteSyntaxTree *child_ = new ConcreteSyntaxTree;
              child_->node_type = (*parse_return.child_symbols)[i_];

              symbol_stack.push(child_);
              tree_ptr->children_.push_back(child_);
              if (i_ == 0)
                break;
            }
            continue;
          }
          break;
          case ParseReturn<CrossTerminal>::Action::OBSERVE_ERROR:
            return false;
          default:
            return false;
        }
      }

      return false;
    }

  }
}
