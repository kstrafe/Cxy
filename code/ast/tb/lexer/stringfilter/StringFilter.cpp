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
#include "StringFilter.hpp"

#include <string>


namespace tul { namespace stringfilter {

StringFilter::StringFilter()
{}


void StringFilter::push(char character)
{
	#define putOut() out[0] = character; can_fetch = 1
	#define outQuote() out[0] = '"'; can_fetch = 1
	#define cOut(x) can_fetch = 1; out[0] = x
	switch (state)
	{
		case State::NOTHING:
			switch (character)
			{
				case '\'':
					state = State::SINGLE;
					outQuote();
				break;
				case '"':
					state = State::MULTIPLE;
					outQuote();
				break;
				case '`':
					state = State::VERBATIM;
					outQuote();
				break;
				default: putOut(); break;
			}
		break;
		case State::SINGLE:
			switch (character)
			{
				case '\'':
					state = State::NOTHING;
					outQuote();
				break;
				case '"':
				case '`':
					can_fetch = 2;
					out[0] = '"';
					out[1] = '"';
				break;
				case '\\':
					previous = state;
					state = State::ESCAPE;
				break;
				default: putOut(); break;
			}
		break;
		case State::MULTIPLE:
			switch (character)
			{
				case '"':
					state = State::NOTHING;
					outQuote();
				break;
				case '\\':
					previous = state;
					state = State::ESCAPE;
				break;
				default: putOut(); break;
			}
		break;
		case State::VERBATIM:
			switch (character)
			{
				case '`':
					state = State::ESCAPE_VERBATIM;
				break;
				case '"':
					can_fetch = 2;
					out[0] = '"';
					out[1] = '"';
				break;
				default: putOut(); break;
			}
		break;
		case State::ESCAPE_VERBATIM:
			switch (character)
			{
				case '`':
					state = State::VERBATIM;
					cOut('`');
					break;
				default:
					state = State::NOTHING;
					can_fetch = 2;
					out[0] = character;
					out[1] = '"';
				break;
			}
		break;
		case State::ESCAPE:
			switch (character)
			{
				case 'u': state = State::CODESMALL; break;
				case 'U': state = State::CODEBIG; break;
				case 'n': state = previous; cOut('\n'); break;
				case 'r': state = previous; cOut('\r'); break;
				case 'f': state = previous; cOut('\f'); break;
				case 'v': state = previous; cOut('\v'); break;
				case 't': state = previous; cOut('\t'); break;
				case '"': state = previous; can_fetch = 2; out[0] = '"'; out[1] = '"'; break;
				default: throw std::string("Can not escape this!"); break;
			}

		break;
	}
	#undef cOut
	#undef putOut
	#undef outQuote
}


bool StringFilter::available() const
{
	return static_cast<bool>(can_fetch);
}


char StringFilter::pop()
{
	--can_fetch;
	return out[can_fetch];
}


void StringFilter::end()
{
	switch (state)
	{
		case State::ESCAPE_VERBATIM:
			out[0] = '"';
			can_fetch = 1;
		default: break;
	}
}

}}
