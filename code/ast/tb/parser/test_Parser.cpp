// Copyright © 2015 Kevin Robert Stravers
/*
This file is part of Cxy Compiler Reference Implementation (Cxy CRI).

Cxy CRI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Cxy CRI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cxy CRI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "libraries/catch.hpp"
#include "Parser.hpp"
#include "protocols/Token.hpp"

#include <map>
#include <vector>


TEST_CASE("Test parser", "[test-Parser]")
{
	SECTION("Make sure that the root object has ENTER as type")
	{
		tul::parser::Parser parser;
		std::unique_ptr<tul::protocols::SyntaxTree> tree = parser.getSyntaxTree();
		REQUIRE(tree->node_type == tul::protocols::CrossTerminal::ENTER);
	}
	SECTION("Try parsing simple a simple expression")
	{
		using namespace tul::protocols;
		tul::parser::Parser parser;
		auto parseSymbol = [&parser](const TokenType &token_type) -> void {Token token; token.token_type = token_type; REQUIRE(parser.parseSymbol(token) == true);};
		auto parseFail = [&parser](const TokenType &token_type) -> void {Token token; token.token_type = token_type; REQUIRE(parser.parseSymbol(token) == false);};

		parseSymbol(TokenType::KEYWORD_VAR);
		parseSymbol(TokenType::IDENTIFIER_PACKAGE);
		parseSymbol(TokenType::SYMBOL_DOT);
		parseSymbol(TokenType::IDENTIFIER_CLASS);
		parseSymbol(TokenType::IDENTIFIER_VARIABLE);
		parseSymbol(TokenType::SYMBOL_SEMICOLON);

		SECTION("See if we handle an empty stack correctly")
		{
			parseFail(TokenType::SYMBOL_SEMICOLON);
		}
	}
}
