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
#include "TreeBuilder.hpp"
#include "parser/dependency/CrossTerminalToString.hpp"
#include "parser/dependency/TokenTypeToCrossTerminal.hpp"
#include <iostream>

namespace tul
{
  namespace treebuilder
  {
    bool TreeBuilder::buildTree(char character)
    {
      if (lexer_object.insertCharacter(character))
      {
        std::vector<protocols::Token> &token_stack = lexer_object.getTokenStack();
        for (int i = 0; i < token_stack.size(); ++i)
        {
          // std::cout << parser::dependency::CrossTerminalToString::convertToString(parser::dependency::TokenTypeToCrossTerminal::convertToCrossTerminal(token_stack.back().token_type)) << std::endl;
          if (false == parser_object.parseSymbol(token_stack[token_stack.size() - 1]))
          {
            std::cout << "Parser failed" << std::endl;
            return false;
          }
          token_stack.pop_back();
        }
        token_stack.clear();
        return true;
      }
      else
      {
        std::cout << "Lexer failed" << std::endl;
        return false;
      }
      std::cout << "Lexer failed" << std::endl;
      return false;
    }

    protocols::ConcreteSyntaxTree *TreeBuilder::getConcreteSyntaxTree()
    {
      return parser_object.getConcreteSyntaxTree();
    }

    std::vector<std::string> TreeBuilder::getExpectedTokens()
    {
      return parser_object.formulateExpectedTokens();
    }

  }
}
