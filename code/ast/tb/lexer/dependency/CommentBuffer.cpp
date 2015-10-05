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

#include <cassert>


namespace tul { namespace dependency {

CommentBuffer::CommentBuffer()
:
	cycle_buffer(CYCLE_BUFFER_SIZE)
{}


void CommentBuffer::putCharInto(char char_)
{
	cycle_array[++cycle_buffer] = char_;
}


char CommentBuffer::getCharFrom(uint8_t location_)
{
	switch (location_)
	{
		case 1: return cycle_array[cycle_buffer.getNumber()];
		case 2: return cycle_array[cycle_buffer.getPrevious()];
		case 0: default: assert(false);
	}
}

}}
