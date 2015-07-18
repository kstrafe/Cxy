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
#include "libraries/catch.hpp"
#include "Parser.hpp"
#include "protocols/Token.hpp"

#include <map>
#include <vector>


TEST_CASE("Test parser", "[test-Parser]")
{
  SECTION("Try parsing simple a simple expression")
  {
    using namespace tul::protocols;
    tul::parser::Parser parser_;
    auto parseSymbol = [&parser_](const TokenType &token_type) -> void {Token token_; token_.token_type = token_type; REQUIRE(parser_.parseSymbol(token_) == true);};
    auto parseFail = [&parser_](const TokenType &token_type) -> void {Token token_; token_.token_type = token_type; REQUIRE(parser_.parseSymbol(token_) == false);};

    parseSymbol(TokenType::KEYWORD_PUBLIC);
    parseSymbol(TokenType::IDENTIFIER_CLASS);
    parseSymbol(TokenType::IDENTIFIER_VARIABLE);
    parseSymbol(TokenType::SYMBOL_SEMICOLON);

    SECTION("See if we handle an empty stack correctly")
    {
      parseFail(TokenType::SYMBOL_SEMICOLON);
    }
  }
}
