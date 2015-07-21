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
#include <cstddef>

namespace tul { namespace dependency {

/**
  The purpose of this class is to count where we currently are positioned.
  It does so by counting each character. New lines will reset the column
  count and increment the line count. In the future we'll need to make
  sure that UTF-8 sequences are counted correctly. Not by size but by
  their apparent length in a monospaced font.
*/
class PositionCounter
{
public:

  void countCharacter(char character_);
  std::size_t getCurrentColumnNumber() const;
  std::size_t getCurrentLineNumber() const;

private:

  std::size_t current_line_number = 1, current_column = 1, utf_bytes_left = 0;

};

}}
