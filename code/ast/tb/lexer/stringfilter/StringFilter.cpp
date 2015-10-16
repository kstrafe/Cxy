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


uint8_t StringFilter::hexToBits(char hex)
{
	uint8_t to_ret = 0;
	switch (hex)
	{
		case '0': to_ret |= 0b0000; break;
		case '1': to_ret |= 0b0001; break;
		case '2': to_ret |= 0b0010; break;
		case '3': to_ret |= 0b0011; break;
		case '4': to_ret |= 0b0100; break;
		case '5': to_ret |= 0b0101; break;
		case '6': to_ret |= 0b0110; break;
		case '7': to_ret |= 0b0111; break;
		case '8': to_ret |= 0b1000; break;
		case '9': to_ret |= 0b1001; break;
		case 'A': to_ret |= 0b1010; break;
		case 'B': to_ret |= 0b1011; break;
		case 'C': to_ret |= 0b1100; break;
		case 'D': to_ret |= 0b1101; break;
		case 'E': to_ret |= 0b1110; break;
		case 'F': to_ret |= 0b1111; break;
		default: throw std::string("Invalid code point"); break;
	}
	return to_ret;
}


void StringFilter::push(char character)
{
	#define putOut() out[0] = character; can_fetch = 1
	#define outQuote() out[0] = '`'; can_fetch = 1
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
				case '`':
					can_fetch = 2;
					out[0] = '`';
					out[1] = '`';
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
				case '`':
					can_fetch = 2;
					out[0] = '`';
					out[1] = '`';
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
				default: putOut(); break;
			}
		break;
		case State::ESCAPE_VERBATIM:
			switch (character)
			{
				case '`':
					state = State::VERBATIM;
					can_fetch = 2;
					out[0] = '`';
					out[1] = '`';
					break;
				default:
					state = State::NOTHING;
					can_fetch = 2;
					out[0] = character;
					out[1] = '`';
				break;
			}
		break;
		case State::ESCAPE:
			switch (character)
			{
				case 'a': state = previous; cOut('\a'); break;
				case 'b': state = previous; cOut('\b'); break;
				case 'c': state = State::CODE; break;
				case 'f': state = previous; cOut('\f'); break;
				case 'n': state = previous; cOut('\n'); break;
				case 'r': state = previous; cOut('\r'); break;
				case 't': state = previous; cOut('\t'); break;
				case 'v': state = previous; cOut('\v'); break;
				case '"': state = previous; can_fetch = 1; out[0] = '"'; break;
				case '\'': state = previous; can_fetch = 1; out[0] = '\''; break;
				default: throw std::string("Can not escape this!"); break;
			}
		break;
		case State::CODE:
			out[0] = 0;
			out[0] |= hexToBits(character) << 4;
			state = State::CODE2;
		break;
		case State::CODE2:
			out[0] |= hexToBits(character);
			if (out[0] == '`')
			{
				out[1] = '`';
				can_fetch = 2;
			}
			else
				can_fetch = 1;
			state = previous;
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
			out[0] = '`';
			can_fetch = 1;
		default: break;
	}
}

}}
