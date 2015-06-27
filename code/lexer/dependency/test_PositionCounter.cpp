#include "PositionCounter.hpp"

#include "libraries/catch.hpp"

#include <string>

TEST_CASE("Test the position counter", "[test-PositionCounter]")
{
  SECTION("See if the position counter counts normal asci codes correctly.")
  {
    tul::lexer::dependency::PositionCounter position_counter;
    for (auto &character_ : std::string("This is a (very) normal and expected piece of text\nWorry not\nThe future is green."))
    {
      position_counter.countCharacter(character_);
    }
    REQUIRE(position_counter.getCurrentColumnNumber() == 21);
    REQUIRE(position_counter.getCurrentLineNumber() == 3);
  }

  SECTION("Now on some non-ascii stuff...")
  {
    tul::lexer::dependency::PositionCounter position_counter;
    for (auto &character_ : std::string("Jeg liker å fôre hunden min."))
    {
      position_counter.countCharacter(character_);
    }
    REQUIRE(position_counter.getCurrentColumnNumber() == 29);
    REQUIRE(position_counter.getCurrentLineNumber() == 1);
  }

  SECTION("Repeating non-ascii characters must be accounted for")
  {
    tul::lexer::dependency::PositionCounter position_counter;
    for (auto &character_ : std::string("å"))
    {
      CHECK(static_cast<unsigned char>(character_) >= 128);
      position_counter.countCharacter(character_);
    }
    REQUIRE(position_counter.getCurrentColumnNumber() == 2);
    REQUIRE(position_counter.getCurrentLineNumber() == 1);
  }

  SECTION("Test counting moonrunes")
  {
    tul::lexer::dependency::PositionCounter position_counter;
    for (auto &character_ : std::string("ᚠᛇᚻ᛫ᛒᛦᚦ"))
    {
      CHECK(static_cast<unsigned char>(character_) >= 128);
      position_counter.countCharacter(character_);
    }
    REQUIRE(position_counter.getCurrentColumnNumber() == 8);
    REQUIRE(position_counter.getCurrentLineNumber() == 1);
  }

  SECTION("Some character sets mixed together")
  {
    tul::lexer::dependency::PositionCounter position_counter;
    for (auto &character_ : std::string("еднᚠᛇᚻῶσσ᛫ᛒმხσსნაᛦHelloᚦ"))
    {
      position_counter.countCharacter(character_);
    }
    REQUIRE(position_counter.getCurrentColumnNumber() == 25);
    REQUIRE(position_counter.getCurrentLineNumber() == 1);
  }

  SECTION("Subramaniya Bharathiyar's poem")
  {
    tul::lexer::dependency::PositionCounter position_counter;
    for (auto &character_ : std::string("யாமறிந்த மொழிகளிலே தமிழ்மொழி போல் இனிதாவது எங்கும் காணோம்"))
    {
      position_counter.countCharacter(character_);
    }
    REQUIRE(position_counter.getCurrentColumnNumber() == 58);
    REQUIRE(position_counter.getCurrentLineNumber() == 1);
  }

  SECTION("Some code points that do not follow strict monospace representation")
  {
    tul::lexer::dependency::PositionCounter position_counter;
    for (auto &character_ : std::string("٩(͡๏̯͡๏)۶   ٩(-̮̮̃•̃)."))
    {
      position_counter.countCharacter(character_);
    }
    REQUIRE(position_counter.getCurrentColumnNumber() == 23);
    REQUIRE(position_counter.getCurrentLineNumber() == 1);
  }

}
