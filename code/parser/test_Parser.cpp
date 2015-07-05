#include "Parser.hpp"

#include "protocols/Token.hpp"

#include "libraries/catch.hpp"

#include <map>
#include <vector>

TEST_CASE("Test parser", "[test-Parser]")
{
  SECTION("Try parsing simple a simple expression")
  {
    using namespace tul::protocols;
    tul::parser::Parser parser_;
    auto parseSymbol = [&parser_](const TokenType &token_type) -> void {Token token_; token_.token_type = token_type; REQUIRE(parser_.parseSymbol(token_) == true);};
    parseSymbol(TokenType::KEYWORD_PUBLIC);
    parseSymbol(TokenType::IDENTIFIER_CLASS);
    parseSymbol(TokenType::IDENTIFIER_VARIABLE);
    parseSymbol(TokenType::SYMBOL_SEMICOLON);
  }
}
