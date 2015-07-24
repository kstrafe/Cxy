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


namespace tul { namespace dependency {

class CommentIgnorer
{
public:

  CommentIgnorer();
  int putOnStack(char character_);
  const char *getCharacterStack() const;

private:

  enum class State
  {
    BLOCK_COMMENT,
    LINE_COMMENT,
    NO_COMMENT,
    INSIDE_QUOTE,
    UNBLOCK_COMMENT,
  }
  comment_state;
  static constexpr const char ring_size = 3;
  char char_stack[ring_size];
  utilities::RingNumber<char> char_at;

};

}}
