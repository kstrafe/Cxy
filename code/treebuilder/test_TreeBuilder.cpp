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

#include "libraries/catch.hpp"

#include <iostream>


TEST_CASE("TreeBuilder must build concrete syntax trees", "[test-TreeBuilder]")
{
  SECTION("Attempting to create a small tree of one node (enter)")
  {
    using namespace tul::treebuilder;

    TreeBuilder builder_object;
    std::string input_string = "private Class x_; public String identifier_name; ";
    for (auto input_character : input_string)
    {
      // Note that each character is validated, and aded to the tree if it completes
      // a token. Data structure:
      /*
        string: current building token. (characters are appended)
        TokenTree
        | --> TokenTree
              | --> TokenTree
        | --> TokenTree --> TokenTree

        The tokentree is a tree of tokens. Once the current building token
        is finished, it's appended to the tree by the internal parser.
      */
      REQUIRE(builder_object.buildTree(input_character));
    }

    /*
      Eventually, we want to get that tree out:
    */
    REQUIRE(builder_object.getConcreteSyntaxTree() != nullptr);
  }
}
