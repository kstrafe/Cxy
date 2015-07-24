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
#include "CommentIgnorer.hpp"

#include <cassert>
#include <iostream>
#include <string>


///////////////////////////////////////////////////////////////////////////////
// The API is quite simple. It is demonstrated and tested here.
////////////////////////////////////////////////////////////////////////////////
TEST_CASE("Ignore comments correctly", "[test-CommentIgnorer]")
{
  using namespace tul::dependency;

  CommentIgnorer com_ign;

  for (char in_char : std::string("Hi! /* don't read this */ Welcome"))
  {
    uint8_t action_code = com_ign.putOnStack(in_char);
    std::cout << "Action: " << static_cast<int>(action_code) << std::endl;
    switch (action_code)
    {
      case 0:
        break;
      case 1:
        std::cout << com_ign.getCharacters().first_char;
        break;
      case 2:
        std::cout
          << com_ign.getCharacters().sec_char
          << com_ign.getCharacters().first_char;
        break;
      default:
        std::cout << "Value defaulted!" << std::endl;
    }
  }
}
