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
#include "protocols/Action.hpp"
#include "protocols/Token.hpp"

#include <cstddef>
#include <string>
#include <vector>


namespace tul { namespace dependency {

/**
	The token generator uses the actions generated by the ActionGenerator
	in order to put characters into tokens. When the token is ready is
	pushes the token into a vector and signals via consumeCharacter that
	a certain number of tokens have been added.
*/
class TokenGenerator
{
public:

	std::size_t consumeCharacter(char character_, protocols::Action action_);
	std::vector<protocols::Token> &getTokenStack();

private:

	std::vector<protocols::Token> token_stack;
	std::string        current_working_lexeme;

};

}}
