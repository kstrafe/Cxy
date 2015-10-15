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
#include "dependency/CommentBuffer.hpp"
#include "dependency/CommentIgnorer.hpp"


namespace tul { namespace filter {

/*
	The data is placed into slots, imagine this comes in sequence to insertCharacter
	`ab//cd':

		[,]   \
		[a,]  a
		[a,b] b
		[/,b] /
		[/,/] /
		[c,/] c
		[c,d] d

	The CommentBuffer holds this. The CommentIgnorer holds state.
	Note how the CommentIgnorer returns a number. 1 means `take the character that
	was just sent in'.
	2 means `take the previous character, and the current one'.

	When is 2 needed? Imagine

	[/, ]

	We can not know for sure if this will be a `/*' comment, so in this case the CommentIgnorer
	returns 0, which means that it has not decided to take anything out yet. When a non
	star comes around, it can safely take out two elements. If it is another slash,
	then the state of the CommentIgnorer will return 0 until a newline. If the next
	symbol is a star, the CommentIgnorer will go into a block-comment mode.
	This code is rather cryptic, so I should move it to a class.
	CommentFilter filter;
	filter.push(char);
	while (filter.available())
		filter.pop();
*/
class CommentFilter
{
public:

	// Always clear the available before pushing again.
	void push(char symbol);
	bool available() const;
	char pop();

private:

	dependency::CommentBuffer comment_buffer;
	dependency::CommentIgnorer comment_ignorer;
	uint8_t symbols_waiting = 0;

};

}}
