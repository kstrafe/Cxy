#include "TokenGenerator.hpp"

#include "libraries/catch.hpp"
#include "protocols/Action.hpp"

#include <string>

TEST_CASE("Test the token generator", "[test-TokenGenerator]")
{
  using namespace tul::protocols;
  tul::lexer::dependency::TokenGenerator token_generator;
  SECTION("No tokens ought to be put on the stack")
  //enum class Action { N, E, P, PTG, TAPTG, TA, TAP, TRP, TR, TRPTG, TSP, TSPTG, TS  };
  {
    #define cons(action) token_generator.consumeCharacter('a', Action::action)
    for (int i_ = 0; i_ < 100; ++i_)
    {
      cons(N);
      cons(E);
      if (i_ % 3 == 0)
        cons(P);
      cons(E);
      if (i_ % 5 == 0)
        cons(N);
      cons(P);
    }
    #undef cons
    REQUIRE(token_generator.getTokenStack().size() == 0);
  }
  SECTION("Suddenly we get a massive almost 600-char token of 'a'")
  {
    REQUIRE(token_generator.consumeCharacter(' ', Action::TA) == 1);
    REQUIRE(token_generator.getTokenStack().size() == 1);
  }
  SECTION("For some reason the client requests only transfers! Let's see if it holds up...")
  {
    for (int i_ = 0; i_ < 100; ++i_)
    {
      REQUIRE(token_generator.consumeCharacter(static_cast<char>(i_ % 256), Action::TAP) == 1);
    }
    REQUIRE(token_generator.getTokenStack().size() == 100 + 1 - 1); // Remember +1 from previous, but also remember that the last one pushed is not transfered
  }
}
