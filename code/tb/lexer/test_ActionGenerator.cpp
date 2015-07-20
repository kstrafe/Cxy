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
#include "actgen/ActionGenerator.hpp"
#include "dependency/Mealy.hpp"
#include "libraries/catch.hpp"
#include "protocols/EntryType.hpp"

#include <string>


TEST_CASE("Test the action generator", "[test-ActionGenerator]")
{
  using namespace tul::actgen;
  using namespace tul::protocols;
  using ac = tul::protocols::Action;
  // enum class EntryType { ALPHA_DIGIT_OR_UNDERSCORE, GROUPING_SYMBOL, QUOTE_SYMBOL, OTHER_SYMBOL, UNKNOWN_CODE_POINT, WHITESPACE };
  ActionGenerator<tul::dependency::Mealy<std::size_t, tul::protocols::Action, tul::protocols::EntryType>> action_generator;
  #define al() action_generator.computeAction(EntryType::ALPHA_DIGIT_OR_UNDERSCORE)
  #define gr() action_generator.computeAction(EntryType::GROUPING_SYMBOL)
  #define qu() action_generator.computeAction(EntryType::QUOTE_SYMBOL)
  #define os() action_generator.computeAction(EntryType::OTHER_SYMBOL)
  #define uc() action_generator.computeAction(EntryType::UNKNOWN_CODE_POINT)
  #define ws() action_generator.computeAction(EntryType::WHITESPACE)
  {
    // This matches (excluding the outer single quotes) 'aa"(!å"""('
    REQUIRE(al() == ac::P);
    REQUIRE(al() == ac::P);
    REQUIRE(qu() == ac::TA);
    REQUIRE(gr() == ac::P);
    REQUIRE(os() == ac::P);
    REQUIRE(uc() == ac::P);
    REQUIRE(ws() == ac::P);
    REQUIRE(qu() == ac::N);
    REQUIRE(qu() == ac::P);
    REQUIRE(qu() == ac::N);
    REQUIRE(gr() == ac::TRPTG);
    REQUIRE(os() == ac::P);
    REQUIRE(os() == ac::P);
    REQUIRE(al() == ac::TSP);
  }
  #undef ws
  #undef uc
  #undef os
  #undef qu
  #undef gr
  #undef al
}
