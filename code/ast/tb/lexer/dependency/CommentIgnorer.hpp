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
#pragma once

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

	CommentIgnorer();
	uint8_t endInput();
	uint8_t putOnStack(char character_);

private:

	enum class State
	{
		BLOCK_COMMENT,
		LINE_COMMENT,
		ONE_SLASH,
		ONE_STAR,
		NO_COMMENT,
		INSIDE_QUOTE,
		MAYBE_OUTQUOTE,
	}
	comment_state;
	std::size_t block_nest = 0;

};

}}
