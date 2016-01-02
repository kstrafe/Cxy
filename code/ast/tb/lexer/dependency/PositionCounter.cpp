// Copyright © 2015-2016 Kevin Robert Stravers
/*
This file is part of Cxy Compiler Reference Implementation (Cxy CRI).

Cxy CRI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Cxy CRI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cxy CRI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "PositionCounter.hpp"


namespace tul { namespace dependency {

void PositionCounter::countCharacter(char character)
{
	/**
		We need to count UTF-8 monospace character length (the amount of code points). This is the perceived.
		length to our eyes. The following table describes how UTF-8 works:

			Bits First     Last       Bytes Byte 1   Byte 2   Byte 3   Byte 4   Byte 5   Byte 6
			 7   U+0000    U+007F     1     0xxxxxxx
			11   U+0080    U+07FF     2     110xxxxx 10xxxxxx
			16   U+0800    U+FFFF     3     1110xxxx 10xxxxxx 10xxxxxx
			21   U+10000   U+1FFFFF   4     11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			26   U+200000  U+3FFFFFF  5     111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			31   U+4000000 U+7FFFFFFF 6     1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

		What the code does is that it inspects the first bits and determines the length. The length determines how
		many of the following characters we have to discard because they're part of the same character.

		We will assume that valid UTF-8 is put into this function. Note that some character sequences make
		the string look smaller than it actually is: "٩(͡๏̯͡๏)۶   ٩(-̮̮̃•̃)". does this, for example.
		We do not need to get that involved into exact character counting. What we do here count the
		number of code points. Many languages map simply and do not require special consideration.
	*/
	if (utf_bytes_left > 0)
	{
		--utf_bytes_left;
		if (utf_bytes_left == 0)
		{
			current_column += 1;
		}
		return;
	}

	if (static_cast<unsigned char>(character) & 0b10000000)
	{
		if (static_cast<unsigned char>(character) & 0b01000000)
		{
			if (static_cast<unsigned char>(character) & 0b00100000)
			{
				if (static_cast<unsigned char>(character) & 0b00010000)
				{
					if (static_cast<unsigned char>(character) & 0b00001000)
					{
						if (static_cast<unsigned char>(character) & 0b00000100)
						{
							utf_bytes_left = 5;
						}
						else
						{
							utf_bytes_left = 4;
						}
					}
					else
					{
						utf_bytes_left = 3;
					}
				}
				else
				{
					utf_bytes_left = 2;
				}
			}
			else
			{
				utf_bytes_left = 1;
			}
		}
		else
		{
			utf_bytes_left = 0; // This is essentially an invalid UTF-8 head. We assume that the input is correct so this will not execute.
		}
	}
	else
	{
		if (character == '\n')
		{
			current_line_number += 1;
			current_column = 1;
		}
		else
		{
			current_column += 1;
		}
	}
}

std::size_t PositionCounter::getCurrentColumnNumber() const
{
	return current_column;
}

std::size_t PositionCounter::getCurrentLineNumber() const
{
	return current_line_number;
}

}}
