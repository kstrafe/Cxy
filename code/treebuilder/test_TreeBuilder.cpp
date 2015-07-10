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


namespace
{
  bool validate(const std::string &string)
  {
    using namespace tul::treebuilder;

    TreeBuilder builder_object;
    bool ret_val = true;
    for (auto input_character : string)
    {
      if (builder_object.buildTree(input_character) == false)
      {
        ret_val = false;
        break;
      }
    }
    if (ret_val == false)
    {
      std::vector<std::string> expected = builder_object.getExpectedTokens();
      std::cout << "\nError: expected:\n";
      for (std::string &string : expected)
        std::cout << string << ", " << std::endl;
    }
    return ret_val;
  }
}


TEST_CASE("TreeBuilder must validate input", "[test-TreeBuilder]")
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

  SECTION("Try parsing the optional assignments")
  {
    using namespace tul::treebuilder;

    TreeBuilder builder_object;
    std::string input_string = "private 32u x_ = 3;";
    for (auto input_character : input_string)
    {
      bool x = false;
      CHECK((x = builder_object.buildTree(input_character)));
      if (!x)
      {
        std::vector<std::string> expected = builder_object.getExpectedTokens();
        std::cout << "\nError: expected:\n";
        for (std::string &string : expected)
        {
          std::cout << string << ", " << std::endl;
        }
        REQUIRE(false);
      }
    }
  }

  SECTION("Try parsing the optional assignments")
  {
    REQUIRE(validate("restricted String str_ = "));
    REQUIRE(validate("restricted String str_ = \"Is this also good?\" ; "));
    REQUIRE(validate("restricted String str_ = \"Is this also good?\" + \"\nHopefully we can add strings together!\"; "));
    REQUIRE(validate("restricted String str_ = \"Is this also good?\" + \"\nHopefully we can add strings together!\" + \"AND EVEN \"\"MORE\"\"\"; "));
  }

  SECTION("Check non-data class with expressions")
  {
    REQUIRE(validate("public (:) "));
    REQUIRE(validate("public (:) enterProgram { } "));
    REQUIRE(validate("public (:) enterProgram { a_ = \"Hi!\"; } "));
    REQUIRE(validate("public (:) enterProgram { a_ = \"Hi!\"; b_ = 300; } "));
    REQUIRE(validate("public (:) enterProgram { a_ = \"Hi!\"; b_ = 300; b_a_ =  b_; } "));
    REQUIRE(validate("public (:) enterProgram { a_ = \"Hi!\"; b_ = 300; b_a_ = 10 * b_; } "));
    REQUIRE(validate("public (:) enterProgram { b_a_ = 100 && 10 * b_ + 5 / 3 - another_identifier; } "));
    REQUIRE(validate("public (:) enterProgram { b_a_ = 100 && 10 * b_ + 5 / 3 - another_identifier | \"Cool m8\"; } "));
    REQUIRE(validate("public (:) enterProgram { b_a_ = sampleFunction(); } "));
    REQUIRE(validate("public (:) enterProgram { b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction() - another_identifier | \"Cool m8\"; } "));
    REQUIRE
    (
      validate
      (
        R"(
          public (:) enterProgram
          {
            b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction() - another_identifier | "Cool m8";
            q_a_d = 100 && 10 * b_ + 5 / 3 * sampleFunction() - another_identifier | "Cool m8";
            b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction() - another_identifier | "Cool m8";
            b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction() ^sampleFunction() - another_identifier | "Cool m8";
            Type b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction() ^sampleFunction() - another_identifier | "Cool m8";
            800u b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction() ^sampleFunction() - another_identifier | "Cool m8";
            8s b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction() ^sampleFunction() - another_identifier | "Cool m8";
            AnotherClassName b_a_ = 100 > 5 && 10 * b_ + 5 / 3 * sampleFunction() ^sampleFunction() - another_identifier | "Cool m8";
          }
        )"
      )
    );
    REQUIRE(validate("public (:) enterProgram { const ClassName "));
    REQUIRE(validate("public (:) enterProgram { const ptr ClassName alpha_; }"));
    REQUIRE(false == validate("public (:) enterProgram { const const ClassName alpha_; }"));
    REQUIRE(validate("public (:) enterProgram { const ptr const ClassName alpha_; }"));
  }

  SECTION("A class using different statement types")
  {
    REQUIRE
    (
      validate
      (
        R"(
          public (:) enterProgram
          {
            32u max_ = 1000;
            32u counter_ = 0;
            while (counter_ < 1000)
            {
              if (counter_ > 500 + 20)
              { printSomething(); }
              for (const 33s cuda_ = 10; cuda_ < 20; ++cuda_;)
              {}
            }
          }
        )"
      )
    );
  }

}
