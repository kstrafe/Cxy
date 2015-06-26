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
      #define caze(x) REQUIRE(token_stack.at(iterator++).string == x)
        caze("Example");
        caze("test");
        caze("input");
        caze("to");
        caze("see");
        caze("if");
        caze("alphanumeric");
        caze(",");
        caze("especially");
        caze("with");
        caze("numbers");
        caze("like");
        caze("123");
        caze("work");
        caze("for");
        caze("the");
        caze("lexer");
      #undef caze
      REQUIRE(token_stack.size() == iterator);
    }
    SECTION("Check if the types are correct")
    {
      using namespace tul::protocols;
      unsigned iterator = 0;
      #define caze(x) REQUIRE(token_stack.at(iterator++).token_type == TokenType::x)
        caze(IDENTIFIER_CLASS);
        caze(IDENTIFIER_PACKAGE);
        caze(IDENTIFIER_PACKAGE);
        caze(IDENTIFIER_PACKAGE);
        caze(IDENTIFIER_PACKAGE);
        caze(KEYWORD_IF);
        caze(IDENTIFIER_PACKAGE);
        caze(SYMBOL_COMMA);
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
      REQUIRE(token_stack.size() == iterator);
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
    REQUIRE(token_stack.at(0).token_type == tul::protocols::TokenType::STRING);
  }
  SECTION("Confirm the different alphanumeric identifies")
  {
    tul::lexer::Lexer lexer;
    for (char character : std::string("You 1s _cant_ just give 4u upYouGo!"))
      lexer.insertCharacter(/*character :*/ character);

    std::vector<tul::protocols::Token> &token_stack = lexer.getTokenStack();

    REQUIRE(token_stack.size() == 7);
    SECTION("Check if the types are correct")
    {
      using namespace tul::protocols;
      unsigned iterator = 0;
      #define caze(x) REQUIRE(token_stack.at(iterator++).token_type == TokenType::x)
        caze(IDENTIFIER_CLASS);
        caze(PRIMITIVE_SIGNED);
        caze(IDENTIFIER_VARIABLE);
        caze(IDENTIFIER_PACKAGE);
        caze(IDENTIFIER_PACKAGE);
        caze(PRIMITIVE_UNSIGNED);
        caze(IDENTIFIER_SUBROUTINE);
      #undef caze
      REQUIRE(token_stack.size() == iterator);
    }
  }
  SECTION("Ensure that the groupers work")
  {
    tul::lexer::Lexer lexer;
    for (char character : std::string("([[You {]} 1s) _cant_ just give{[(]])]} 4u upYouGo!"))
      lexer.insertCharacter(/*character :*/ character);

    std::vector<tul::protocols::Token> &token_stack = lexer.getTokenStack();

    REQUIRE(token_stack.size() == 22);
    SECTION("Check if the types are correct")
    {
      using namespace tul::protocols;
      unsigned iterator = 0;
      #define caze(x) REQUIRE(token_stack.at(iterator++).token_type == TokenType::x)
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
      REQUIRE(token_stack.size() == iterator);
    }
  }
  SECTION("Check if symbols are lexed")
  {
    tul::lexer::Lexer lexer;
    for (char character : std::string("Muh * + sym ++bols/\\"))
      lexer.insertCharacter(/*character :*/ character);

    std::vector<tul::protocols::Token> &token_stack = lexer.getTokenStack();

    REQUIRE(token_stack.size() == 6);
    SECTION("Check if the types are correct")
    {
      using namespace tul::protocols;
      unsigned iterator = 0;
      #define caze(x) REQUIRE(token_stack.at(iterator++).token_type == TokenType::x)
        caze(IDENTIFIER_CLASS);
        caze(SYMBOL_STAR);
        caze(SYMBOL_PLUS);
        caze(IDENTIFIER_PACKAGE);
        caze(SYMBOL_PLUS_PLUS);
        caze(IDENTIFIER_PACKAGE);
      #undef caze
      REQUIRE(token_stack.size() == iterator);
    }
  }
  SECTION("Are keywords detected?")
  {
    tul::lexer::Lexer lexer;
    for (char character : std::string("Muh * if()while() assembly{} + sym ++bols/\\"))
      lexer.insertCharacter(/*character :*/ character);

    std::vector<tul::protocols::Token> &token_stack = lexer.getTokenStack();

    REQUIRE(token_stack.size() == 15);
    using namespace tul::protocols;
    unsigned iterator = 0;
    #define caze(x) REQUIRE(token_stack.at(iterator++).token_type == TokenType::x)
      caze(IDENTIFIER_CLASS);
      caze(SYMBOL_STAR);
      caze(KEYWORD_IF);
      caze(GROUPER_LEFT_PARENTHESIS);
      caze(GROUPER_RIGHT_PARENTHESIS);
      caze(KEYWORD_WHILE);
      caze(GROUPER_LEFT_PARENTHESIS);
      caze(GROUPER_RIGHT_PARENTHESIS);
      caze(KEYWORD_ASSEMBLY);
      caze(GROUPER_LEFT_BRACE);
      caze(GROUPER_RIGHT_BRACE);
      caze(SYMBOL_PLUS);
      caze(IDENTIFIER_PACKAGE);
      caze(SYMBOL_PLUS_PLUS);
      caze(IDENTIFIER_PACKAGE);
    #undef caze
    REQUIRE(token_stack.size() == iterator);
  }
}
