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


////////////////////////////////////////////////////////////////////////////////
// This is only a small utility validator... if you want to see how
// TreeBuilder works just go down to "TEST_CASE"
////////////////////////////////////////////////////////////////////////////////
namespace
{
  bool validate(const std::string &string, bool print_error_if_exists = true)
  {
    using namespace tul::treebuilder;

    TreeBuilder builder_object;
    bool ret_val = true;
    char current_ = '\0';
    for (auto input_character : string)
    {
      if (builder_object.buildTree(input_character) == false)
      {
        ret_val = false;
        break;
      }
      current_ = input_character;
    }
    if (ret_val == false && print_error_if_exists)
    {
      std::cout << "last char: " << current_ << std::endl;
      std::vector<std::string> expected = builder_object.getExpectedTokens();
      std::cout << "\nError: expected:\n";
      for (std::string &string : expected)
        std::cout << string << ", " << std::endl;
    }
    return ret_val;
  }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The API is quite simple. It is demonstrated and tested here.
////////////////////////////////////////////////////////////////////////////////
TEST_CASE("TreeBuilder must validate input", "[test-TreeBuilder]")
{
  using namespace tul::treebuilder;

  SECTION("Attempting to validate a small program")
  {
    // Create an instance and feed characters into it.
    // Every time a character is fed the return value
    // tells you if the input is still valid.
    TreeBuilder builder_object;
    std::string input_string = "private ClassName variable_name; public String identifier_name; ";
    // Note the trailing space in the string. This causes the TreeBuilder to be able
    // To turn ; into a token, and thereby finish the input. If we left it out
    // The ; would merely be in a "current_working" state. Nothing would be invalidated,
    // It's just that we would not see it in the tree.
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

    // Eventually, we may want to get that tree
    REQUIRE(builder_object.getConcreteSyntaxTree() != nullptr);
  }
  ////////////////////////////////////////////////////////////////////////////////
  SECTION("Try parsing the optional assignments")
  {
    REQUIRE(validate("private 32u value_ = 9000;"));
    REQUIRE(validate("public 64s value_ = -9000;"));
  }
  ////////////////////////////////////////////////////////////////////////////////
  SECTION("Try parsing the optional assignments")
  {
    REQUIRE(validate("restricted String str_ = "));
    REQUIRE(validate("restricted String str_ = \"Is this also good?\" ; "));
    REQUIRE(validate("restricted String str_ = \"Is this also good?\" + \"\nHopefully we can add strings together!\"; "));
    REQUIRE(validate("restricted String str_ = \"Is this also good?\" + \"\nHopefully we can add strings together!\" + \"AND EVEN \"\"MORE\"\"\"; "));
  }
  ////////////////////////////////////////////////////////////////////////////////
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
    REQUIRE(validate(R"(
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
    )"));
    REQUIRE(validate("public (:) enterProgram { const ClassName "));
    REQUIRE(validate("public (:) enterProgram { const ptr ClassName alpha_; }"));
    REQUIRE(false == validate("public (:) enterProgram { const const ClassName alpha_; }", false));
    REQUIRE(validate("public (:) enterProgram { const ptr const ClassName alpha_; }"));
  }
  ////////////////////////////////////////////////////////////////////////////////
  SECTION("A class using different statement types")
  {
    REQUIRE(validate(R"(
          public (:) enterProgram
          {
            32u max_ = 1000;
            32u counter_ = 0;
            while (counter_ < 1000)
            {
              if (counter_ > 500 + 20)
              { printSomething(); }
              for (const 33s cuda_ = 10; cuda_ < 20; ++cuda_; --cuda_;)
              {
                ++global_var;
              }
            }
          }
    )"));
  }
  ////////////////////////////////////////////////////////////////////////////////
  SECTION("Multiple function definitions")
  {
    ////////////////////////////////////////////////////////////
    REQUIRE(validate(R"(
          restricted 32u variable_ = 0;

          public (:) enterProgram
          {
            ++variable_;
          }

          private (:) performCalculation
          {
            --variable_;
          }
    )"));
    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    REQUIRE(validate(R"(
            restricted 32u variable_ = 0;

            public (:) enterProgram
            {
              ++variable_;
            }

            private (:) performCalculation
            {
              --variable_;
            }
    )"));
    ////////////////////////////////////////////////////////////
  }
  ////////////////////////////////////////////////////////////
  SECTION("Unary operators")
  {
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              32u a_ = 1000;
              ptr 32u b_ = $a_;
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              32u a_ = 1000;
              ptr 32u b_ = $a_;
              ++@b_;
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              32u a_ = 1000;
              ptr 32u b_ = $$a_;
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              32u a_ = 1000;
              ptr const 32u b_ = $$a_;
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              32u a_ = 1000;
              ptr const 32u b_ = $$a_;
              32u c_ = @@b_;
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              32u a_ = 1000;
              ptr 32u b_ = $a_;
              ++ @b_;
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              32u a_ = 1000;
              ptr 32u b_ = $a_;
              if (!! (@b_ > 100))
              {

              }
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              32u a_ = 1000;
              ptr 32u b_ = $a_;
              if (!! (@b_ > 100))
              {

              }
            }

            public (: : pure) funcTion
            {

            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              32u a_ = 1000;
              ptr 32u b_ = $a_;
              if (!! (@b_ > 100))
              {

              }
            }

            public (: : const) strToInt
            {}

            public (: : const pure) funcTion
            {}
    )"));
  }
  ////////////////////////////////////////////////////////////
  SECTION("Argument Lists")
  {
    REQUIRE(validate(R"(
            public (32u a_:32u b_) myFunction
            {}
    )"));
    REQUIRE(validate(R"(
            public (32u a_, Class b_:32u c_) myFunction
            {}
    )"));
    REQUIRE(validate(R"(
            public (32u a_, Class b_ : Class c_, 32u d_) myFunction
            {}
    )"));
    REQUIRE(validate(R"(
            public (32u a_, b_ : Class c_, c2_, 32u d_, e_) myFunction
            {}
    )"));
  }
  ////////////////////////////////////////////////////////////
  SECTION("Array types")
  {
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              [3, 32u] arr_;
            }
    )"));
  }
}
