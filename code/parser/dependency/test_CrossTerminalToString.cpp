#include "CrossTerminalToString.hpp"

#include "libraries/catch.hpp"

#include <iostream>
#include <vector>


TEST_CASE("Cross-terminal to string representation", "[test-CrossTerminalToString]")
{
  for (int i = 0; i < static_cast<int>(tul::protocols::CrossTerminal::ENUM_END); ++i)
    REQUIRE(tul::parser::dependency::CrossTerminalToString::convertToString(static_cast<tul::protocols::CrossTerminal>(i)) != "");
  REQUIRE(tul::parser::dependency::CrossTerminalToString::convertToString(tul::protocols::CrossTerminal::ENUM_END) == "");
}
