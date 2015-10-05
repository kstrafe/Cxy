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
#include "TreeBuilder.hpp"


namespace tul { namespace tb {

bool TreeBuilder::buildTree(char character)
{
	if (lexer_object.insertCharacter(character))
	{
		std::vector<protocols::Token> &token_stack = lexer_object.getTokenStack();
		for (std::size_t i = 0; i < token_stack.size(); ++i)
		{
			if (false == parser_object.parseSymbol(token_stack[i]))
			{
				return false;
			}
		}
		token_stack.clear();
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

bool TreeBuilder::endInput()
{
	buildTree(' '); // Use a space to clear out the last symbol

	protocols::Token token_;
	token_.token_type = protocols::TokenType::END_OF_MODULE;

	if (false == parser_object.parseSymbol(token_))
	{
		return false;
	}
	return parser_object.isEmpty();
}

std::unique_ptr<protocols::ConcreteSyntaxTree> TreeBuilder::getConcreteSyntaxTree()
{
	return parser_object.getConcreteSyntaxTree();
}

std::vector<std::string> TreeBuilder::getExpectedTokens()
{
	return parser_object.formulateExpectedTokens();
}

}}
