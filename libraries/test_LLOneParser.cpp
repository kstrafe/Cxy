#include "libraries/LLOneParser.hpp"
#include "libraries/catch.hpp"

#include <iostream>

TEST_CASE("Test the kernel of the LL(1) parser", "[test-LL1]")
{

  SECTION("Try parsing a counting grammar of (), where each entry must match and line up.")
  {
    enum Symbols
    {
      LPAREN, RPAREN,
      GROUPER, POSGROUPER,
      END
    };
    tul::libraries::LLOneParser<Symbols> ll_parser;
    ll_parser.addRule(GROUPER, LPAREN, {LPAREN, POSGROUPER, RPAREN, POSGROUPER});
    ll_parser.addRule(POSGROUPER, LPAREN, {LPAREN, POSGROUPER, RPAREN, POSGROUPER});
    ll_parser.addEpsilon(POSGROUPER);

    ll_parser.pushSymbol(END);
    ll_parser.pushSymbol(GROUPER);

    std::vector<Symbols> input_symbols
    // ((())())
    {LPAREN, LPAREN, LPAREN, RPAREN, RPAREN, LPAREN, RPAREN, RPAREN, END};

    for (Symbols input_symbol : input_symbols)
    {
      tul::libraries::LLOneParser<Symbols>::ParseReturn current_return {nullptr, false};
      do
      {
        current_return = ll_parser.parseSymbol(input_symbol);
        CHECK(current_return.is_error == false);
      }
      while (current_return.was_removed == false && current_return.is_error == false);
    }
  }

  SECTION("A failure to adhere to the syntax.")
  {
    enum Symbols
    {
      LPAREN, RPAREN,
      GROUPER, POSGROUPER,
      END
    };
    tul::libraries::LLOneParser<Symbols> ll_parser;
    ll_parser.addRule(GROUPER, LPAREN, {LPAREN, POSGROUPER, RPAREN, POSGROUPER});
    ll_parser.addRule(POSGROUPER, LPAREN, {LPAREN, POSGROUPER, RPAREN, POSGROUPER});
    ll_parser.addEpsilon(POSGROUPER);

    ll_parser.pushSymbol(END);
    ll_parser.pushSymbol(GROUPER);

    tul::libraries::LLOneParser<Symbols>::ParseReturn current_return = ll_parser.parseSymbol(END);

    REQUIRE(current_return.is_error == true);
  }

  SECTION("Adding and multiplying numbers.")
  {
    enum Symbols
    {
      LPAREN, RPAREN,
      PLUS, STAR, NUM,

      SUM, PRODUCT,
      MAY_SUM, MAY_PRODUCT,
      GROUPER,
      END
    };
    tul::libraries::LLOneParser<Symbols> ll_parser;
    ll_parser.addRule(SUM, NUM, {PRODUCT, MAY_SUM});
    ll_parser.addRule(SUM, LPAREN, {LPAREN, SUM, RPAREN, MAY_SUM});
    ll_parser.addRule(SUM, PLUS, {PLUS, SUM});
    ll_parser.addRule(PRODUCT, NUM, {NUM, MAY_PRODUCT});
    ll_parser.addRule(MAY_SUM, PLUS, {PLUS, SUM});
    ll_parser.addRule(MAY_SUM, STAR, {STAR, PRODUCT, MAY_SUM});
    ll_parser.addRule(MAY_PRODUCT, STAR, {STAR, PRODUCT});

    ll_parser.addEpsilon(MAY_SUM);
    ll_parser.addEpsilon(MAY_PRODUCT);

    ll_parser.pushSymbol(END);
    ll_parser.pushSymbol(SUM);

    std::vector<Symbols> input_symbols
    {LPAREN, NUM, PLUS, NUM, RPAREN, STAR, NUM, END};

    for (Symbols input_symbol : input_symbols)
    {
      tul::libraries::LLOneParser<Symbols>::ParseReturn current_return {nullptr, false};
      do
      {
        current_return = ll_parser.parseSymbol(input_symbol);
        CHECK(current_return.is_error == false);
      }
      while (current_return.was_removed == false && current_return.is_error == false);
    }

  }

}
