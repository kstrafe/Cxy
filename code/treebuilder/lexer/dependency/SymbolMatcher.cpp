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
#include "SymbolMatcher.hpp"


namespace tul
{
	namespace treebuilder
	{
		namespace lexer
		{
			namespace dependency
			{
				protocols::TokenType SymbolMatcher::getSymbol(const std::string &lexeme)
				{
					using namespace protocols;
					if (lexeme == "~") return protocols::TokenType::SYMBOL_TILDE;
					else if (lexeme == "||") return protocols::TokenType::SYMBOL_BAR__BAR;
					else if (lexeme == "|") return protocols::TokenType::SYMBOL_BAR;
					else if (lexeme == "^") return protocols::TokenType::SYMBOL_CARET;
					else if (lexeme == "\\") return protocols::TokenType::SYMBOL_BACKWARD_SLASH;
					else if (lexeme == "@@") return protocols::TokenType::SYMBOL_APETAIL__APETAIL;
					else if (lexeme == "@") return protocols::TokenType::SYMBOL_APETAIL;
					else if (lexeme == ">>") return protocols::TokenType::SYMBOL_GREATER_THAN__GREATER_THAN;
					else if (lexeme == ">=") return protocols::TokenType::SYMBOL_GREATER_THAN__EQUAL;
					else if (lexeme == ">") return protocols::TokenType::SYMBOL_GREATER_THAN;
					else if (lexeme == "==") return protocols::TokenType::SYMBOL_EQUAL__EQUAL;
					else if (lexeme == "=") return protocols::TokenType::SYMBOL_EQUAL;
					else if (lexeme == "<=") return protocols::TokenType::SYMBOL_LESS_THAN__EQUAL;
					else if (lexeme == "<<") return protocols::TokenType::SYMBOL_LESS_THAN__LESS_THAN;
					else if (lexeme == "<") return protocols::TokenType::SYMBOL_LESS_THAN;
					else if (lexeme == ";") return protocols::TokenType::SYMBOL_SEMICOLON;
					else if (lexeme == ":") return protocols::TokenType::SYMBOL_COLON;
					else if (lexeme == "/") return protocols::TokenType::SYMBOL_FORWARD_SLASH;
					else if (lexeme == "..") return protocols::TokenType::SYMBOL_DOT__DOT;
					else if (lexeme == ".") return protocols::TokenType::SYMBOL_DOT;
					else if (lexeme == "--") return protocols::TokenType::SYMBOL_MINUS__MINUS;
					else if (lexeme == "-") return protocols::TokenType::SYMBOL_MINUS;
					else if (lexeme == ",") return protocols::TokenType::SYMBOL_COMMA;
					else if (lexeme == "++") return protocols::TokenType::SYMBOL_PLUS__PLUS;
					else if (lexeme == "+") return protocols::TokenType::SYMBOL_PLUS;
					else if (lexeme == "**") return protocols::TokenType::SYMBOL_STAR__STAR;
					else if (lexeme == "*") return protocols::TokenType::SYMBOL_STAR;
					else if (lexeme == "&&") return protocols::TokenType::SYMBOL_AMPERSAND__AMPERSAND;
					else if (lexeme == "&") return protocols::TokenType::SYMBOL_AMPERSAND;
					else if (lexeme == "$$") return protocols::TokenType::SYMBOL_DOLLAR__DOLLAR;
					else if (lexeme == "$") return protocols::TokenType::SYMBOL_DOLLAR;
					else if (lexeme == "!!") return protocols::TokenType::SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK;
					else if (lexeme == "!") return protocols::TokenType::SYMBOL_EXCLAMATION_MARK;
					else return protocols::TokenType::UNIDENTIFIED;
				}
			}
		}
	}
}