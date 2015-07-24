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
#include "CommentIgnorer.hpp"


namespace tul { namespace dependency {

CommentIgnorer::CommentIgnorer()
:
  comment_state(State::NO_COMMENT),
  char_at(ring_size)
{
  char_stack[static_cast<std::size_t>(char_at.getNumber())] = '\0';
}


uint8_t CommentIgnorer::putOnStack(char character_)
{
  char previous_character = char_stack[static_cast<std::size_t>(char_at.getNumber())];
  char_stack[static_cast<std::size_t>(++char_at)] = character_;

  switch (comment_state)
  {
    case State::NO_COMMENT:
    {
      switch (character_)
      {
        case '/':
          switch (previous_character)
          {
            case '/': comment_state = State::LINE_COMMENT; break;
            case '*': comment_state = --block_nest == 0 ? State::NO_COMMENT : State::BLOCK_COMMENT; break;
            default: break;
          }
        break;
        case '*':
          switch (previous_character)
          {
            case '/': comment_state = State::BLOCK_COMMENT; ++block_nest; break;
            default: break;
          }
        break;
        case '"':
          switch (comment_state)
          {
            case State::INSIDE_QUOTE: comment_state = State::NO_COMMENT; break;
            case State::NO_COMMENT: comment_state = State::INSIDE_QUOTE; break;
            default: break;
          }
        break;
        default: return 1; break;
      }
    }
    break;
    case State::LINE_COMMENT:
      switch (character_)
      {
        case '\n': comment_state = State::NO_COMMENT; break;
        default: break;
      }

    break;
    default: break;
  }

  return 0;
}

CommentIgnorer::ReturnCharacters CommentIgnorer::getCharacters() const
{
  char previous_character = char_stack[static_cast<std::size_t>(char_at.getPrevious())];
  char curr_character = char_stack[static_cast<std::size_t>(char_at.getNumber())];
  ReturnCharacters ret_ch;
  ret_ch.sec_char = previous_character;
  ret_ch.first_char = curr_character;
  return ret_ch;
}

}}
