#include "KeywordMatcher.hpp"

#include "libraries/catch.hpp"

TEST_CASE("Test keyword matcher", "[test-KeywordMatcher]")
{
  SECTION("Attempting to match a few keywords")
  {
    using namespace tul::lexer::dependency;
    using namespace tul::protocols;
    auto checkReturn = [](const std::string &lexeme) -> bool { return  KeywordMatcher::getKeyword(lexeme) != TokenType::UNIDENTIFIED; };
    REQUIRE(checkReturn("if"));
    REQUIRE(checkReturn("while"));
    REQUIRE(checkReturn("do"));
  }
}
