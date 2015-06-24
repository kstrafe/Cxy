#include "Lexer.hpp"

#include "protocols/Token.hpp"
#include "protocols/TokenType.hpp"

#include "libraries/catch.hpp"

TEST_CASE("Test lexer output", "[test-Lexer]")
{
  SECTION("Lexing a natural language string")
  {
    tul::lexer::Lexer lexer;
    for (char character : std::string("Example test input to see if alphanumeric, especially with numbers like 123 work for the lexer."))
      lexer.insertCharacter(/*character :*/ character);

    std::vector<tul::protocols::Token> &token_stack = lexer.getTokenStack();

    REQUIRE(token_stack.size() == 17);

    SECTION("Check if the lexemes are correct")
    {
      unsigned iterator = 0;
      REQUIRE(token_stack.at(iterator++).string == "Example");
      REQUIRE(token_stack.at(iterator++).string == "test");
      REQUIRE(token_stack.at(iterator++).string == "input");
      REQUIRE(token_stack.at(iterator++).string == "to");
      REQUIRE(token_stack.at(iterator++).string == "see");
      REQUIRE(token_stack.at(iterator++).string == "if");
      REQUIRE(token_stack.at(iterator++).string == "alphanumeric");
      REQUIRE(token_stack.at(iterator++).string == ",");
      REQUIRE(token_stack.at(iterator++).string == "especially");
      REQUIRE(token_stack.at(iterator++).string == "with");
      REQUIRE(token_stack.at(iterator++).string == "numbers");
      REQUIRE(token_stack.at(iterator++).string == "like");
      REQUIRE(token_stack.at(iterator++).string == "123");
      REQUIRE(token_stack.at(iterator++).string == "work");
      REQUIRE(token_stack.at(iterator++).string == "for");
      REQUIRE(token_stack.at(iterator++).string == "the");
      REQUIRE(token_stack.at(iterator++).string == "lexer");
    }
    SECTION("Check if the types are correct")
    {
      using namespace tul::protocols;
      unsigned iterator = 0;
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_CLASS);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::UNIDENTIFIED);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::INTEGER_LITERAL);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
      REQUIRE(token_stack.at(iterator++).token_type == TokenType::IDENTIFIER_VARIABLE_OR_SUBROUTINE);
    }
  }
  SECTION("Check if the lexer lexes string literals correctly")
  {
    tul::lexer::Lexer lexer;
    for (char character : std::string("\"Example test input to see if alphanumeric, especially with numbers like 123 work for the lexer.\" "))
      lexer.insertCharacter(/*character :*/ character);

    std::vector<tul::protocols::Token> &token_stack = lexer.getTokenStack();

    REQUIRE(token_stack.size() == 1);
    REQUIRE(token_stack.at(0).string == "Example test input to see if alphanumeric, especially with numbers like 123 work for the lexer.");
  }
}
