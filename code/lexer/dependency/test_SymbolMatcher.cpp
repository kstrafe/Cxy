#include "SymbolMatcher.hpp"

#include "libraries/catch.hpp"

TEST_CASE("Test symbol matcher", "[test-SymbolMatcher]")
{
  SECTION("Attempting to match a few symbols")
  {
    using namespace tul::lexer::dependency;
    using namespace tul::protocols;
    auto checkReturn = [](const std::string &lexeme) -> bool { return  SymbolMatcher::getSymbol(lexeme) != TokenType::UNIDENTIFIED; };
    REQUIRE(checkReturn("++"));
    REQUIRE(checkReturn("-"));
    REQUIRE(checkReturn("^"));
    REQUIRE(!checkReturn("if"));
  }
}
