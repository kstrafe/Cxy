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
#include "../../tb/TreeBuilder.hpp"
#include "libraries/catch.hpp"
#include "TreePruner.hpp"
#include "protocols/CrossTerminalTools.hpp"

#include <cassert>
#include <iostream>


std::string printTree(const tul::protocols::ConcreteSyntaxTree *cst_, int indent = 0)
{
  using namespace tul;
  {
    std::stringstream str_strm;
    str_strm << std::setfill('0') << std::setw(3) << indent << ':';
    std::string ind(indent, ' ');
    ind += str_strm.str();
    ind += protocols::CrossTerminalTools::toString(cst_->node_type);
    ind += '(';
    ind += cst_->token_.accompanying_lexeme;
    ind += ')';
    ind += '\n';
    for (auto child_ : cst_->children_)
        ind += printTree(child_, indent + 2);
    return ind;
  }
}

bool checkSemantics(tul::protocols::ConcreteSyntaxTree *tree_)
{
  // This function gets all the methods of a file.
  using namespace tul::protocols;
  if (tree_->node_type == CrossTerminal::FUNCTION_DEFINITION)
  {
    assert(tree_->children_.size() == 3 || tree_->children_.size() == 2);
    std::cout << "fun: " << tree_->children_[1]->token_.accompanying_lexeme << std::endl;
  }
  if
  (
    tree_->node_type == CrossTerminal::ENTER
    || tree_->node_type == CrossTerminal::DECL_OR_FUNC
    || tree_->node_type == CrossTerminal::FUNCTION_LIST
  )
  {
    for (tul::protocols::ConcreteSyntaxTree *subtree_ : tree_->children_)
    {
      checkSemantics(subtree_);
    }
  }
  return true;
}

bool validate(std::string string, bool print_error_if_exists = true)
{
  // return true;
  tul::tb::TreeBuilder builder_object;
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
  std::unique_ptr<tul::protocols::ConcreteSyntaxTree> tree_;
  if (ret_val)
  {
    tul::TreePruner pruner_;
    tree_ = builder_object.getConcreteSyntaxTree();
    pruner_.pruneTree(tree_.get());
    std::cout << printTree(tree_.get());
    checkSemantics(tree_.get());
  }
  return ret_val;
}

TEST_CASE("Prune a tree", "[test-system]")
{
  REQUIRE(validate(R"(
    public (:) enterProgram
    {
      printLine(str_: "Howdy!" + 2 - 3);
    }
  )"));
  REQUIRE(validate(R"(
    public (:) enterProgram
    {
      if (sys.Out.getStatus()~is_available)
        sys.Out.printLine(line_: "A line");
      else
        @sys.Control.abortProgram > 5;
    }
  )"));
  REQUIRE(validate(R"(
        public (:) enterProgram
        {
          var 32u a_;
        }
        public (:) kekDe
        {

        }
        public (32u zak_: ) eDboii
        { zak_; }
  )"));
  REQUIRE(validate(R"(
        public (:) enterProgram
        {
          var 32u a_ = 100;
          a_ = when (a_ > 100 || doNothing()~k_) 50 else 10;
        }
  )"));
}