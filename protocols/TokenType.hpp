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

namespace tul { namespace protocols { 

enum class TokenType
{
    END_OF_MODULE,
    GROUPER_LEFT_BRACE,
    GROUPER_LEFT_BRACKET,
    GROUPER_LEFT_PARENTHESIS,
    GROUPER_RIGHT_BRACE,
    GROUPER_RIGHT_BRACKET,
    GROUPER_RIGHT_PARENTHESIS,
    IDENTIFIER_CLASS,
    IDENTIFIER_ENUMERATION,
    IDENTIFIER_PACKAGE,
    IDENTIFIER_SUBROUTINE,
    IDENTIFIER_VARIABLE,
    INTEGER_LITERAL,
    KEYWORD_CAST,
    KEYWORD_CONST,
    KEYWORD_DEFER,
    KEYWORD_DO,
    KEYWORD_ELSE,
    KEYWORD_FOR,
    KEYWORD_GOTO,
    KEYWORD_GRANT,
    KEYWORD_HACK,
    KEYWORD_IF,
    KEYWORD_LABEL,
    KEYWORD_LAMBDA,
    KEYWORD_PTR,
    KEYWORD_PURE,
    KEYWORD_REF,
    KEYWORD_RETURN,
    KEYWORD_STATIC,
    KEYWORD_VAR,
    KEYWORD_WHEN,
    KEYWORD_WHILE,
    PRIMITIVE_SIGNED,
    PRIMITIVE_UNSIGNED,
    STRING,
    SYMBOL_AMPERSAND,
    SYMBOL_AMPERSAND__AMPERSAND,
    SYMBOL_APETAIL,
    SYMBOL_APETAIL__APETAIL,
    SYMBOL_BAR,
    SYMBOL_BAR__BAR,
    SYMBOL_CARET,
    SYMBOL_COLON__PRUNE,
    SYMBOL_COMMA__PRUNE,
    SYMBOL_DOLLAR,
    SYMBOL_DOLLAR__DOLLAR,
    SYMBOL_DOT__PRUNE,
    SYMBOL_EQUAL,
    SYMBOL_EQUAL__EQUAL,
    SYMBOL_EXCLAMATION_MARK,
    SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK,
    SYMBOL_FORWARD_SLASH,
    SYMBOL_FORWARD_SLASH__EQUAL,
    SYMBOL_GREATER_THAN,
    SYMBOL_GREATER_THAN__EQUAL,
    SYMBOL_LESS_THAN,
    SYMBOL_LESS_THAN__EQUAL,
    SYMBOL_MINUS,
    SYMBOL_MINUS__EQUAL,
    SYMBOL_MINUS__MINUS,
    SYMBOL_PLUS,
    SYMBOL_PLUS__EQUAL,
    SYMBOL_PLUS__PLUS,
    SYMBOL_SEMICOLON__PRUNE,
    SYMBOL_STAR,
    SYMBOL_STAR__EQUAL,
    SYMBOL_TILDE__PRUNE,
    UNIDENTIFIED,
    ENUM_END
};

}}
