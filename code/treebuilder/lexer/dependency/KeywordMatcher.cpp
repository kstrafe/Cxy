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
#include "KeywordMatcher.hpp"


namespace tul
{
	namespace treebuilder
	{
		namespace lexer
		{
			namespace dependency
			{
				protocols::TokenType KeywordMatcher::getKeyword(const std::string &lexeme)
				{
					if (lexeme == "assembly") return protocols::TokenType::KEYWORD_ASSEMBLY;
					else if (lexeme == "do") return protocols::TokenType::KEYWORD_DO;
					else if (lexeme == "for") return protocols::TokenType::KEYWORD_FOR;
					else if (lexeme == "goto") return protocols::TokenType::KEYWORD_GOTO;
					else if (lexeme == "if") return protocols::TokenType::KEYWORD_IF;
					else if (lexeme == "label") return protocols::TokenType::KEYWORD_LABEL;
					else if (lexeme == "private") return protocols::TokenType::KEYWORD_PRIVATE;
					else if (lexeme == "public") return protocols::TokenType::KEYWORD_PUBLIC;
					else if (lexeme == "restricted") return protocols::TokenType::KEYWORD_RESTRICTED;
					else if (lexeme == "while") return protocols::TokenType::KEYWORD_WHILE;
					else return protocols::TokenType::UNIDENTIFIED;
				}
			}
		}
	}
}
