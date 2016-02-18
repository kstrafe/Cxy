// Copyright © 2015-2016 Kevin Robert Stravers
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
#include <string>

namespace match {

template <typename Token, typename TokenType>
class KeywordMatcher {

	static TokenType getKeyword(const std::string &lexeme) {
		if (lexeme == "continue") return TokenType::KEYWORD_CONTINUE;
		else if (lexeme == "foreach") return TokenType::KEYWORD_FOREACH;
		else if (lexeme == "lambda") return TokenType::KEYWORD_LAMBDA;
		else if (lexeme == "return") return TokenType::KEYWORD_RETURN;
		else if (lexeme == "break") return TokenType::KEYWORD_BREAK;
		else if (lexeme == "catch") return TokenType::KEYWORD_CATCH;
		else if (lexeme == "debug") return TokenType::KEYWORD_DEBUG;
		else if (lexeme == "defer") return TokenType::KEYWORD_DEFER;
		else if (lexeme == "label") return TokenType::KEYWORD_LABEL;
		else if (lexeme == "throw") return TokenType::KEYWORD_THROW;
		else if (lexeme == "while") return TokenType::KEYWORD_WHILE;
		else if (lexeme == "cast") return TokenType::KEYWORD_CAST;
		else if (lexeme == "else") return TokenType::KEYWORD_ELSE;
		else if (lexeme == "goto") return TokenType::KEYWORD_GOTO;
		else if (lexeme == "hack") return TokenType::KEYWORD_HACK;
		else if (lexeme == "pure") return TokenType::KEYWORD_PURE;
		else if (lexeme == "size") return TokenType::KEYWORD_SIZE;
		else if (lexeme == "this") return TokenType::KEYWORD_THIS;
		else if (lexeme == "type") return TokenType::KEYWORD_TYPE;
		else if (lexeme == "when") return TokenType::KEYWORD_WHEN;
		else if (lexeme == "del") return TokenType::KEYWORD_FOR;
		else if (lexeme == "for") return TokenType::KEYWORD_FOR;
		else if (lexeme == "new") return TokenType::KEYWORD_NEW;
		else if (lexeme == "try") return TokenType::KEYWORD_TRY;
		else if (lexeme == "do") return TokenType::KEYWORD_DO;
		else if (lexeme == "if") return TokenType::KEYWORD_IF;
		else if (lexeme == "in") return TokenType::KEYWORD_IN;
		else return TokenType::UNIDENTIFIED;
	}

};

}
