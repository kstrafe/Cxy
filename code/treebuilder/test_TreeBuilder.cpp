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

#include "protocols/CrossTerminal.hpp"
#include "protocols/ParseReturn.hpp"

#include "libraries/catch.hpp"

#include <iostream>


////////////////////////////////////////////////////////////////////////////////
// This is only a small utility validator... if you want to see how
// TreeBuilder works just go down to "TEST_CASE"
////////////////////////////////////////////////////////////////////////////////
namespace
{
  std::string printTree(const tul::protocols::ConcreteSyntaxTree *cst_, int indent = 0)
  {
    std::stringstream str_strm;
    str_strm << std::setfill('0') << std::setw(3) << indent << ':';
    using namespace tul::treebuilder::parser::dependency;
    std::string ind(indent, ' ');
    ind += str_strm.str();
    ind += CrossTerminalToString::convertToString(cst_->node_type);
    ind += '(';
    ind += cst_->token_.accompanying_lexeme;
    ind += ')';
    ind += '\n';
    for (auto child_ : cst_->children_)
    {
      if (child_->node_type != tul::protocols::CrossTerminal::EPSILONATE)
        ind += printTree(child_, indent + 2);
    }
    return ind;
  }

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
    if (ret_val)
      ret_val = builder_object.endInput();
    if (ret_val == false && print_error_if_exists)
    {
      std::cout << "last char: " << current_ << std::endl;
      std::vector<std::string> expected = builder_object.getExpectedTokens();
      std::cout << "\nError: expected:\n";
      for (std::string &string : expected)
        std::cout << string << ", " << std::endl;
    }
    // std::cout << printTree(builder_object.getConcreteSyntaxTree()) << std::endl;
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
    REQUIRE(false == validate("restricted String str_ = 0", false));
    REQUIRE(validate("restricted String str_ = \"Is this also good?\" ; "));
    REQUIRE(validate("restricted String str_ = \"Is this also good?\" + \"\nHopefully we can add strings together!\"; "));
    REQUIRE(validate("restricted String str_ = \"Is this also good?\" + \"\nHopefully we can add strings together!\" + \"AND EVEN \"\"MORE\"\"\"; "));
  }
  ////////////////////////////////////////////////////////////////////////////////
  SECTION("Check non-data class with expressions")
  {
    REQUIRE(false == validate("public (:) ", false));
    REQUIRE(validate("public (:) enterProgram { } "));
    REQUIRE(validate("public (:) enterProgram { a_ = \"Hi!\"; } "));
    REQUIRE(validate("public (:) enterProgram { a_ = \"Hi!\"; b_ = 300; } "));
    REQUIRE(validate("public (:) enterProgram { a_ = \"Hi!\"; b_ = 300; b_a_ =  b_; } "));
    REQUIRE(validate("public (:) enterProgram { a_ = \"Hi!\"; b_ = 300; b_a_ = 10 * b_; } "));
    REQUIRE(validate("public (:) enterProgram { b_a_ = 100 && 10 * b_ + 5 / 3 - another_identifier; } "));
    REQUIRE(validate("public (:) enterProgram { b_a_ = 100 && 10 * b_ + 5 / 3 - another_identifier | \"Cool m8\"; } "));
    REQUIRE(validate("public (:) enterProgram { b_a_ = sampleFunction()~a_; } "));
    REQUIRE(validate("public (:) enterProgram { b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction()~a_ - another_identifier | \"Cool m8\"; } "));
    REQUIRE(validate(R"(
          public (:) enterProgram
          {
            b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction()~a_ - another_identifier | "Cool m8";
            q_a_d = 100 && 10 * b_ + 5 / 3 * sampleFunction()~a_ - another_identifier | "Cool m8";
            b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction()~a_ - another_identifier | "Cool m8";
            b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction()~a_ ^sampleFunction()~a_ - another_identifier | "Cool m8";
            var Type b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction()~a_ ^sampleFunction()~a_ - another_identifier | "Cool m8";
            var 800u b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction()~a_ ^sampleFunction()~a_ - another_identifier | "Cool m8";
            var 8s b_a_ = 100 && 10 * b_ + 5 / 3 * sampleFunction()~a_ ^sampleFunction()~a_ - another_identifier | "Cool m8";
            var AnotherClassName b_a_ = 100 > 5 && 10 * b_ + 5 / 3 * sampleFunction()~a_ ^sampleFunction()~a_ - another_identifier | "Cool m8";
          }
    )"));
    REQUIRE(false == validate("public (:) enterProgram { var const ClassName ", false));
    REQUIRE(validate("public (:) enterProgram { var const ptr ClassName alpha_; }"));
    REQUIRE(false == validate("public (:) enterProgram { var const const ClassName alpha_; }", false));
    REQUIRE(validate("public (:) enterProgram { var const ptr const ClassName alpha_; }"));
  }
  ////////////////////////////////////////////////////////////////////////////////
  SECTION("A class using different statement types")
  {
    REQUIRE(validate(R"(
          public (:) enterProgram
          {
            var 32u max_ = 1000;
            var 32u counter_ = 0;
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
              var 32u a_ = 1000;
              var ptr 32u b_ = $a_;
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              var 32u a_ = 1000;
              var ptr 32u b_ = $a_;
              ++@b_;
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              var 32u a_ = 1000;
              var ptr 32u b_ = $$a_;
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              var 32u a_ = 1000;
              var ptr const 32u b_ = $$a_;
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              var 32u a_ = 1000;
              var ptr const 32u b_ = $$a_;
              var 32u c_ = @@b_;
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              var 32u a_ = 1000;
              var ptr 32u b_ = $a_;
              ++ @b_;
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              var 32u a_ = 1000;
              var ptr 32u b_ = $a_;
              if (!! (@b_ > 100))
              {

              }
            }
    )"));
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              var 32u a_ = 1000;
              var ptr 32u b_ = $a_;
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
              var 32u a_ = 1000;
              var ptr 32u b_ = $a_;
              if (!! (@b_ > 100))
              {

              }
            }

            public (: : const) strToInt
            {}

            public (: : const pure) funcTion
            {}


            public (:: const) strToInte
            {}

            public (:: const pure) funcTione
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
    REQUIRE(validate(R"(
            public ([10, 32u] a_, b_ : Class c_, c2_, 32u d_, e_) myFunction
            {}
    )"));
  }
  ////////////////////////////////////////////////////////////
  SECTION("Array types")
  {
    REQUIRE(validate(R"(
            public (:) enterProgram
            {
              var [3, 32u] arr_;
              arr_[1] = 200;
            }

            public (:) anotherFunction
            {
              var [2, [3, [4, [5, 8u]]]] szt_;
            }
    )"));
  }
  ////////////////////////////////////////////////////////////
  SECTION("Putting variables in the middle is not allowed")
  {
    REQUIRE(false == validate(R"(
            public (:) enterProgram
            {}

            public 32u var_;
    )", false));
  }
  ////////////////////////////////////////////////////////////
  SECTION("Type casting")
  {
    REQUIRE(validate(R"(
          public (:) enterProgram
          {
            var 32u a_ = 100;
            var 16u b_ = cast(16u) { a_ } * 5;
          }
    )"));
  }
  ////////////////////////////////////////////////////////////
  SECTION("Member expression")
  {
    REQUIRE(validate(R"(
          public (:) enterProgram
          {
            var SomeClass a_ = SomeClass.createInstance()~instance_;
            var SomeClass b_ = SomeClass.something_()~ck_;
            var SomeClass c_ = sys.Cool.something_()~ck_()~x_[3 + 8];
            var Qs d_ = q.Qz.ctOr()~obj_;
          }
    )"));
    REQUIRE(validate(R"(
          public (:) enterProgram
          {
            var Aa a_ = Aa();
          }
    )"));
    REQUIRE(validate(R"(
          public (:) enterProgram
          {
            var a.Aa a_ = Aa();
            a_ = b_;
          }
    )"));
    REQUIRE(validate(R"(
          public (:) enterProgram
          {
            var a.Aa a_ = a.Aa();
          }
    )"));
    REQUIRE(false == validate(R"(
          public (:) enterProgram
          {
            var a.Aa a_ = a.b.Aa();
          }
    )", false));
    REQUIRE(false == validate(R"(
          public (:) enterProgram
          {
            var a.b.Aa a_ = a.Aa();
          }
    )", false));
    REQUIRE(validate(R"(
          public (:) enterProgram
          {
            var sys.Dir dir_(start_: "/");
            var sml.String paths_;
            while (dir_.hasChild()~yes_)
            {
               paths_ += dir_.getCurrent()~child_[1 + f_()~g_[a.Aa.h_()~j_()~k_[l_]]];
            }

            a.Aa.g_ += f_;
          }
    )"));
    REQUIRE(false == validate(R"(
          public (:) enterProgram
          {
            a_ = b_ = c_;
          }
    )", false));
  }
}
