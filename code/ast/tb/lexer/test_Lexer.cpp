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
#include "Lexer.hpp"
#include "libraries/catch.hpp"
#include "protocols/Token.hpp"
#include "protocols/TokenType.hpp"


TEST_CASE("Test lexer output", "[test-Lexer]")
{
	SECTION("Lexing a natural language string")
	{
		tul::lexer::Lexer lexing_engine;
		for (char character : std::string("Example test input to see if alphanumeric, especially with numbers like 123 work for the lexer."))
			lexing_engine.insertCharacter(/*character :*/ character);

		std::vector<tul::protocols::Token> &token_stack = lexing_engine.getTokenStack();

		REQUIRE(token_stack.size() == 17);

		SECTION("Check if the lexemes are correct")
		{
			unsigned iterator_ = 0;
			#define caze(string_) REQUIRE(token_stack.at(iterator_).accompanying_lexeme == string_)
			#define caze2(col_number) REQUIRE(token_stack.at(iterator_++).column_number == col_number)
				caze("Example"); caze2(8);
				caze("test"); caze2(13);
				caze("input"); caze2(19);
				caze("to"); caze2(22);
				caze("see"); caze2(26);
				caze("if"); caze2(29);
				caze("alphanumeric"); caze2(42);
				caze(","); caze2(43);
				caze("especially"); caze2(54);
				caze("with"); caze2(59);
				caze("numbers"); caze2(67);
				caze("like"); caze2(72);
				caze("123"); caze2(76);
				caze("work"); caze2(81);
				caze("for"); caze2(85);
				caze("the"); caze2(89);
				caze("lexer"); caze2(95);
			#undef caze2
			#undef caze
			REQUIRE(token_stack.size() == iterator_);
		}
		SECTION("Check if the types are correct")
		{
			using namespace tul::protocols;
			unsigned iterator_ = 0;
			#define caze(type_name) REQUIRE(token_stack.at(iterator_++).token_type == TokenType::type_name)
				caze(IDENTIFIER_CLASS);
				caze(IDENTIFIER_PACKAGE);
				caze(IDENTIFIER_PACKAGE);
				caze(IDENTIFIER_PACKAGE);
				caze(IDENTIFIER_PACKAGE);
				caze(KEYWORD_IF);
				caze(IDENTIFIER_PACKAGE);
				caze(SYMBOL_COMMA__PRUNE);
				caze(IDENTIFIER_PACKAGE);
				caze(IDENTIFIER_PACKAGE);
				caze(IDENTIFIER_PACKAGE);
				caze(IDENTIFIER_PACKAGE);
				caze(INTEGER_LITERAL);
				caze(IDENTIFIER_PACKAGE);
				caze(KEYWORD_FOR);
				caze(IDENTIFIER_PACKAGE);
				caze(IDENTIFIER_PACKAGE);
			#undef caze
			REQUIRE(token_stack.size() == iterator_);
		}
	}


	SECTION("Check if the lexer lexes string literals correctly")
	{
		tul::lexer::Lexer lexing_engine;
		for (char character : std::string("\"Example test input to see if alphanumeric, especially with numbers like 123 work for the lexer.\" "))
			lexing_engine.insertCharacter(/*character :*/ character);

		std::vector<tul::protocols::Token> &token_stack = lexing_engine.getTokenStack();

		REQUIRE(token_stack.size() == 1);
		REQUIRE(token_stack.at(0).accompanying_lexeme == "Example test input to see if alphanumeric, especially with numbers like 123 work for the lexer.");
		REQUIRE(token_stack.at(0).token_type == tul::protocols::TokenType::STRING);
		REQUIRE(token_stack.at(0).column_number == 98);
	}


	SECTION("Confirm the different alphanumeric identifies")
	{
		tul::lexer::Lexer lexing_engine;
		for (char character : std::string("You 1s _cant_ just give 4u upYouGo!"))
			lexing_engine.insertCharacter(/*character :*/ character);

		std::vector<tul::protocols::Token> &token_stack = lexing_engine.getTokenStack();

		REQUIRE(token_stack.size() == 7);
		SECTION("Check if the types are correct")
		{
			using namespace tul::protocols;
			unsigned iterator_ = 0;
			#define caze(type_name) REQUIRE(token_stack.at(iterator_).token_type == TokenType::type_name)
			#define caze2(col_number) REQUIRE(token_stack.at(iterator_++).column_number == col_number)
				caze(IDENTIFIER_CLASS); caze2(4);
				caze(PRIMITIVE_SIGNED); caze2(7);
				caze(IDENTIFIER_VARIABLE); caze2(14);
				caze(IDENTIFIER_PACKAGE); caze2(19);
				caze(IDENTIFIER_PACKAGE); caze2(24);
				caze(PRIMITIVE_UNSIGNED); caze2(27);
				caze(IDENTIFIER_SUBROUTINE); caze2(35);
			#undef caze2
			#undef caze
			REQUIRE(token_stack.size() == iterator_);
		}
	}


	SECTION("Ensure that the groupers work")
	{
		tul::lexer::Lexer lexing_engine;
		for (char character : std::string("([[You {]} 1s) _cant_ just give{[(]])]} 4u upYouGo!"))
			lexing_engine.insertCharacter(/*character :*/ character);

		std::vector<tul::protocols::Token> &token_stack = lexing_engine.getTokenStack();

		REQUIRE(token_stack.size() == 22);
		SECTION("Check if the types are correct")
		{
			using namespace tul::protocols;
			unsigned iterator_ = 0;
			#define caze(type_name) REQUIRE(token_stack.at(iterator_++).token_type == TokenType::type_name)
				caze(GROUPER_LEFT_PARENTHESIS);
				caze(GROUPER_LEFT_BRACKET);
				caze(GROUPER_LEFT_BRACKET);
				caze(IDENTIFIER_CLASS);
				caze(GROUPER_LEFT_BRACE);
				caze(GROUPER_RIGHT_BRACKET);
				caze(GROUPER_RIGHT_BRACE);
				caze(PRIMITIVE_SIGNED);
				caze(GROUPER_RIGHT_PARENTHESIS);
				caze(IDENTIFIER_VARIABLE);
				caze(IDENTIFIER_PACKAGE);
				caze(IDENTIFIER_PACKAGE);
				caze(GROUPER_LEFT_BRACE);
				caze(GROUPER_LEFT_BRACKET);
				caze(GROUPER_LEFT_PARENTHESIS);
				caze(GROUPER_RIGHT_BRACKET);
				caze(GROUPER_RIGHT_BRACKET);
				caze(GROUPER_RIGHT_PARENTHESIS);
				caze(GROUPER_RIGHT_BRACKET);
				caze(GROUPER_RIGHT_BRACE);
				caze(PRIMITIVE_UNSIGNED);
				caze(IDENTIFIER_SUBROUTINE);
			#undef caze
			REQUIRE(token_stack.size() == iterator_);
		}
	}


	SECTION("Check if symbols are lexed")
	{
		tul::lexer::Lexer lexing_engine;
		for (char character : std::string("Muh * + sym ++bols/\\"))
			lexing_engine.insertCharacter(/*character :*/ character);

		std::vector<tul::protocols::Token> &token_stack = lexing_engine.getTokenStack();

		REQUIRE(token_stack.size() == 6);
		SECTION("Check if the types are correct")
		{
			using namespace tul::protocols;
			unsigned iterator_ = 0;
			#define caze(type_name) REQUIRE(token_stack.at(iterator_++).token_type == TokenType::type_name)
				caze(IDENTIFIER_CLASS);
				caze(SYMBOL_STAR);
				caze(SYMBOL_PLUS);
				caze(IDENTIFIER_PACKAGE);
				caze(SYMBOL_PLUS__PLUS);
				caze(IDENTIFIER_PACKAGE);
			#undef caze
			REQUIRE(token_stack.size() == iterator_);
		}
	}


	SECTION("Are keywords detected?")
	{
		tul::lexer::Lexer lexing_engine;
		for (char character : std::string("Muh * if()while() hack() + sym ++bols/\\"))
			lexing_engine.insertCharacter(/*character :*/ character);

		std::vector<tul::protocols::Token> &token_stack = lexing_engine.getTokenStack();

		REQUIRE(token_stack.size() == 15);
		using namespace tul::protocols;
		unsigned iterator_ = 0;
		#define caze(type_name) REQUIRE(token_stack.at(iterator_++).token_type == TokenType::type_name)
			caze(IDENTIFIER_CLASS);
			caze(SYMBOL_STAR);
			caze(KEYWORD_IF);
			caze(GROUPER_LEFT_PARENTHESIS);
			caze(GROUPER_RIGHT_PARENTHESIS);
			caze(KEYWORD_WHILE);
			caze(GROUPER_LEFT_PARENTHESIS);
			caze(GROUPER_RIGHT_PARENTHESIS);
			caze(KEYWORD_HACK);
			caze(GROUPER_LEFT_PARENTHESIS);
			caze(GROUPER_RIGHT_PARENTHESIS);
			caze(SYMBOL_PLUS);
			caze(IDENTIFIER_PACKAGE);
			caze(SYMBOL_PLUS__PLUS);
			caze(IDENTIFIER_PACKAGE);
		#undef caze
		REQUIRE(token_stack.size() == iterator_);
	}


	SECTION("Deal with UTF-8 code points")
	{
		using namespace tul::protocols;

		tul::lexer::Lexer lexing_engine;
		for (char character : std::string("æøå\u0000\u0001\u0002\u0003\u0004\u0005\u0006\u0007\u0008\u000E\u000F\u0010\u0011\u0012\u0013\u0014\u0015\u0016\u0017\u0018\u0019\u001A\u001B\u001C\u001D\u001E\u001F"))
			REQUIRE (lexing_engine.insertCharacter(/*character :*/ character) == false );
		for (char character : std::string("\"æøå\u0000\u0001\u0002\u0003\u0004\u0005\u0006\u0007\u0008\u000E\u000F\u0010\u0011\u0012\u0013\u0014\u0015\u0016\u0017\u0018\u0019\u001A\u001B\u001C\u001D\u001E\u001F\" "))
			REQUIRE (lexing_engine.insertCharacter(/*character :*/ character) == true );

		std::vector<tul::protocols::Token> &token_stack = lexing_engine.getTokenStack();
		REQUIRE(token_stack.size() == 1);
		REQUIRE(token_stack.at(0).accompanying_lexeme == "æøå\u0000\u0001\u0002\u0003\u0004\u0005\u0006\u0007\u0008\u000E\u000F\u0010\u0011\u0012\u0013\u0014\u0015\u0016\u0017\u0018\u0019\u001A\u001B\u001C\u001D\u001E\u001F");
		REQUIRE(token_stack.at(0).token_type == TokenType::STRING);
	}


	SECTION("Identify data declaration lexemes")
	{
		using namespace tul::lexer;
		Lexer lex_eng;
		for (auto character : std::string("private 32u x_ = 3; "))
			lex_eng.insertCharacter(character);

		std::vector<tul::protocols::Token> &token_stack = lex_eng.getTokenStack();
		REQUIRE(token_stack.size() == 6);
		using namespace tul::protocols;
		unsigned iterator_ = 0;
		#define caze(type_name) REQUIRE(token_stack.at(iterator_++).token_type == TokenType::type_name)
			caze(IDENTIFIER_PACKAGE);
			caze(PRIMITIVE_UNSIGNED);
			caze(IDENTIFIER_VARIABLE);
			caze(SYMBOL_EQUAL);
			caze(INTEGER_LITERAL);
			caze(SYMBOL_SEMICOLON__PRUNE);
		#undef caze
		REQUIRE(token_stack.size() == iterator_);
	}


	SECTION("Identify function declaration lexemes")
	{
		using namespace tul::lexer;
		Lexer lex_eng;
		for (auto character : std::string("public (:)   public (: ) "))
			lex_eng.insertCharacter(character);

		std::vector<tul::protocols::Token> &token_stack = lex_eng.getTokenStack();
		REQUIRE(token_stack.size() == 8);
		unsigned iterator_ = 0;
		for (int i = 0; i < 2; ++i)
		{
			using namespace tul::protocols;
			#define caze(type_name) REQUIRE(token_stack.at(iterator_++).token_type == TokenType::type_name)
				caze(IDENTIFIER_PACKAGE);
				caze(GROUPER_LEFT_PARENTHESIS);
				caze(SYMBOL_COLON__PRUNE);
				caze(GROUPER_RIGHT_PARENTHESIS);
			#undef caze
		}
		REQUIRE(token_stack.size() == iterator_);
	}

	SECTION("Ensure the correct identification of access specifiers")
	{
		using namespace tul::lexer;
		Lexer lex_eng;
		for (auto character : std::string("inp_a "))
			lex_eng.insertCharacter(character);

		std::vector<tul::protocols::Token> &token_stack = lex_eng.getTokenStack();
		REQUIRE(token_stack.size() == 1);
		unsigned iterator_ = 0;
		using namespace tul::protocols;
		#define caze(type_name) REQUIRE(token_stack.at(iterator_++).token_type == TokenType::type_name)
			caze(IDENTIFIER_VARIABLE);
		#undef caze
	}

}
