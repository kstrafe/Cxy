// Copyright © 2015 Kevin Robert Stravers
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
#include "CommentBuffer.hpp"

#include <string>


namespace tul { namespace dependency {

CommentBuffer::CommentBuffer()
:
	cycle_buffer(CYCLE_BUFFER_SIZE)
{}


void CommentBuffer::putCharInto(char character)
{
	cycle_array[++cycle_buffer] = character;
}


char CommentBuffer::getCharFrom(uint8_t location)
{
	switch (location)
	{
		case 1: return cycle_array[cycle_buffer.getNumber()];
		case 2: return cycle_array[cycle_buffer.getPrevious()];
		case 0: default: throw std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": location given is out of bounds.";
	}
}

}}
