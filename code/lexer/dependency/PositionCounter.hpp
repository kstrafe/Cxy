#pragma once
#include <cstddef>

namespace tul
{
  namespace lexer
  {
    namespace dependency
    {
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
    }
  }
}
