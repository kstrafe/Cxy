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
#pragma once
#include "utilities/RingNumber.hpp"

#include <cstddef>
#include <cstdint>


namespace tul { namespace dependency {

/*
  How does this class work?

  Feed it characters. For every character that you put
  into it using putOnStack, you will get a number of
  characters that you may validly extract from
  getCharacterStack();
*/
class CommentIgnorer
{
public:

  struct ReturnCharacters
  {
    char
      first_char,
      sec_char;
  };

  CommentIgnorer();
  uint8_t putOnStack(char character_);
  ReturnCharacters getCharacters() const;

private:

  enum class State
  {
    BLOCK_COMMENT,
    LINE_COMMENT,
    NO_COMMENT,
    INSIDE_QUOTE,
  }
  comment_state;
  static constexpr const char ring_size = 3;
  char char_stack[ring_size];
  std::size_t block_nest = 0;
  utilities::RingNumber<char> char_at;

};

}}
