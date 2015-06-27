#include "Lexer.hpp"

#include "protocols/Token.hpp"
#include "protocols/TokenType.hpp"

#include "libraries/catch.hpp"

TEST_CASE("Test lexer output", "[test-Lexer]")
{
  SECTION("Lexing a natural language string")
  {
    tul::lexer::Lexer lexing_engine;
    for (char character_ : std::string("Example test input to see if alphanumeric, especially with numbers like 123 work for the lexer."))
      lexing_engine.insertCharacter(/*character_ :*/ character_);

    std::vector<tul::protocols::Token> &token_stack = lexing_engine.getTokenStack();

    REQUIRE(token_stack.size() == 17);

    SECTION("Check if the lexemes are correct")
    {
      unsigned iterator_ = 0;
      #define caze(string_) REQUIRE(token_stack.at(iterator_++).accompanying_lexeme == string_)
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
      REQUIRE(token_stack.size() == iterator_);
    }
  }
  SECTION("Check if the lexer lexes string literals correctly")
  {
    tul::lexer::Lexer lexing_engine;
    for (char character_ : std::string("\"Example test input to see if alphanumeric, especially with numbers like 123 work for the lexer.\" "))
      lexing_engine.insertCharacter(/*character_ :*/ character_);

    std::vector<tul::protocols::Token> &token_stack = lexing_engine.getTokenStack();

    REQUIRE(token_stack.size() == 1);
    REQUIRE(token_stack.at(0).accompanying_lexeme == "Example test input to see if alphanumeric, especially with numbers like 123 work for the lexer.");
    REQUIRE(token_stack.at(0).token_type == tul::protocols::TokenType::STRING);
  }
  SECTION("Confirm the different alphanumeric identifies")
  {
    tul::lexer::Lexer lexing_engine;
    for (char character_ : std::string("You 1s _cant_ just give 4u upYouGo!"))
      lexing_engine.insertCharacter(/*character_ :*/ character_);

    std::vector<tul::protocols::Token> &token_stack = lexing_engine.getTokenStack();

    REQUIRE(token_stack.size() == 7);
    SECTION("Check if the types are correct")
    {
      using namespace tul::protocols;
      unsigned iterator_ = 0;
      #define caze(type_name) REQUIRE(token_stack.at(iterator_++).token_type == TokenType::type_name)
        caze(IDENTIFIER_CLASS);
        caze(PRIMITIVE_SIGNED);
        caze(IDENTIFIER_VARIABLE);
        caze(IDENTIFIER_PACKAGE);
        caze(IDENTIFIER_PACKAGE);
        caze(PRIMITIVE_UNSIGNED);
        caze(IDENTIFIER_SUBROUTINE);
      #undef caze
      REQUIRE(token_stack.size() == iterator_);
    }
  }
  SECTION("Ensure that the groupers work")
  {
    tul::lexer::Lexer lexing_engine;
    for (char character_ : std::string("([[You {]} 1s) _cant_ just give{[(]])]} 4u upYouGo!"))
      lexing_engine.insertCharacter(/*character_ :*/ character_);

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
    for (char character_ : std::string("Muh * + sym ++bols/\\"))
      lexing_engine.insertCharacter(/*character_ :*/ character_);

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
        caze(SYMBOL_PLUS_PLUS);
        caze(IDENTIFIER_PACKAGE);
      #undef caze
      REQUIRE(token_stack.size() == iterator_);
    }
  }
  SECTION("Are keywords detected?")
  {
    tul::lexer::Lexer lexing_engine;
    for (char character_ : std::string("Muh * if()while() assembly{} + sym ++bols/\\"))
      lexing_engine.insertCharacter(/*character_ :*/ character_);

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
      caze(KEYWORD_ASSEMBLY);
      caze(GROUPER_LEFT_BRACE);
      caze(GROUPER_RIGHT_BRACE);
      caze(SYMBOL_PLUS);
      caze(IDENTIFIER_PACKAGE);
      caze(SYMBOL_PLUS_PLUS);
      caze(IDENTIFIER_PACKAGE);
    #undef caze
    REQUIRE(token_stack.size() == iterator_);
  }
  SECTION("Deal with UTF-8 code points")
  {
    using namespace tul::protocols;

    tul::lexer::Lexer lexing_engine;
    for (char character_ : std::string("æøå\u0000\u0001\u0002\u0003\u0004\u0005\u0006\u0007\u0008\u000E\u000F\u0010\u0011\u0012\u0013\u0014\u0015\u0016\u0017\u0018\u0019\u001A\u001B\u001C\u001D\u001E\u001F"))
      REQUIRE (lexing_engine.insertCharacter(/*character_ :*/ character_) == false );
    for (char character_ : std::string("\"æøå\u0000\u0001\u0002\u0003\u0004\u0005\u0006\u0007\u0008\u000E\u000F\u0010\u0011\u0012\u0013\u0014\u0015\u0016\u0017\u0018\u0019\u001A\u001B\u001C\u001D\u001E\u001F\" "))
      REQUIRE (lexing_engine.insertCharacter(/*character_ :*/ character_) == true );

    std::vector<tul::protocols::Token> &token_stack = lexing_engine.getTokenStack();
    REQUIRE(token_stack.size() == 1);
    REQUIRE(token_stack.at(0).accompanying_lexeme == "æøå\u0000\u0001\u0002\u0003\u0004\u0005\u0006\u0007\u0008\u000E\u000F\u0010\u0011\u0012\u0013\u0014\u0015\u0016\u0017\u0018\u0019\u001A\u001B\u001C\u001D\u001E\u001F");
    REQUIRE(token_stack.at(0).token_type == TokenType::STRING);
  }
}
