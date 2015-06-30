#include "ActionGenerator.hpp"

#include "libraries/catch.hpp"

#include "protocols/EntryType.hpp"

#include <string>

TEST_CASE("Test the action generator", "[test-ActionGenerator]")
{
  using namespace tul::lexer::dependency;
  using namespace tul::protocols;
  using ac = tul::protocols::Action;
  // enum class EntryType { ALPHA_DIGIT_OR_UNDERSCORE, GROUPING_SYMBOL, QUOTE_SYMBOL, OTHER_SYMBOL, UNKNOWN_CODE_POINT, WHITESPACE };
  ActionGenerator action_generator;
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
