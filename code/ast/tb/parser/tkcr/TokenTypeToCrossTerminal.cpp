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
#include "TokenTypeToCrossTerminal.hpp"
#include "protocols/CrossTerminal.hpp"
#include "protocols/TokenType.hpp"


namespace tul { namespace tkcr { 

protocols::CrossTerminal TokenTypeToCrossTerminal::convertToCrossTerminal(protocols::TokenType token_type)
{
	switch (token_type)
	{
		case protocols::TokenType::END_OF_MODULE: return protocols::CrossTerminal::END_OF_MODULE;
		case protocols::TokenType::GROUPER_LEFT_BRACE: return protocols::CrossTerminal::GROUPER_LEFT_BRACE;
		case protocols::TokenType::GROUPER_LEFT_BRACKET: return protocols::CrossTerminal::GROUPER_LEFT_BRACKET;
		case protocols::TokenType::GROUPER_LEFT_PARENTHESIS: return protocols::CrossTerminal::GROUPER_LEFT_PARENTHESIS;
		case protocols::TokenType::GROUPER_RIGHT_BRACE: return protocols::CrossTerminal::GROUPER_RIGHT_BRACE;
		case protocols::TokenType::GROUPER_RIGHT_BRACKET: return protocols::CrossTerminal::GROUPER_RIGHT_BRACKET;
		case protocols::TokenType::GROUPER_RIGHT_PARENTHESIS: return protocols::CrossTerminal::GROUPER_RIGHT_PARENTHESIS;
		case protocols::TokenType::IDENTIFIER_CLASS: return protocols::CrossTerminal::IDENTIFIER_CLASS;
		case protocols::TokenType::IDENTIFIER_CONSTEXPR: return protocols::CrossTerminal::IDENTIFIER_CONSTEXPR;
		case protocols::TokenType::IDENTIFIER_ENUMERATION: return protocols::CrossTerminal::IDENTIFIER_ENUMERATION;
		case protocols::TokenType::IDENTIFIER_PACKAGE: return protocols::CrossTerminal::IDENTIFIER_PACKAGE;
		case protocols::TokenType::IDENTIFIER_SUBROUTINE: return protocols::CrossTerminal::IDENTIFIER_SUBROUTINE;
		case protocols::TokenType::IDENTIFIER_VARIABLE: return protocols::CrossTerminal::IDENTIFIER_VARIABLE;
		case protocols::TokenType::INTEGER_LITERAL: return protocols::CrossTerminal::INTEGER_LITERAL;
		case protocols::TokenType::KEYWORD_ALIAS: return protocols::CrossTerminal::KEYWORD_ALIAS;
		case protocols::TokenType::KEYWORD_CAST: return protocols::CrossTerminal::KEYWORD_CAST;
		case protocols::TokenType::KEYWORD_CATCH: return protocols::CrossTerminal::KEYWORD_CATCH;
		case protocols::TokenType::KEYWORD_CONST: return protocols::CrossTerminal::KEYWORD_CONST;
		case protocols::TokenType::KEYWORD_CONSTRUCT: return protocols::CrossTerminal::KEYWORD_CONSTRUCT;
		case protocols::TokenType::KEYWORD_DEBUG: return protocols::CrossTerminal::KEYWORD_DEBUG;
		case protocols::TokenType::KEYWORD_DEFER: return protocols::CrossTerminal::KEYWORD_DEFER;
		case protocols::TokenType::KEYWORD_DELETE: return protocols::CrossTerminal::KEYWORD_DELETE;
		case protocols::TokenType::KEYWORD_DO: return protocols::CrossTerminal::KEYWORD_DO;
		case protocols::TokenType::KEYWORD_DOUBLE: return protocols::CrossTerminal::KEYWORD_DOUBLE;
		case protocols::TokenType::KEYWORD_ELSE: return protocols::CrossTerminal::KEYWORD_ELSE;
		case protocols::TokenType::KEYWORD_ENUM: return protocols::CrossTerminal::KEYWORD_ENUM;
		case protocols::TokenType::KEYWORD_FLOAT: return protocols::CrossTerminal::KEYWORD_FLOAT;
		case protocols::TokenType::KEYWORD_FOR: return protocols::CrossTerminal::KEYWORD_FOR;
		case protocols::TokenType::KEYWORD_GOTO: return protocols::CrossTerminal::KEYWORD_GOTO;
		case protocols::TokenType::KEYWORD_GRANT: return protocols::CrossTerminal::KEYWORD_GRANT;
		case protocols::TokenType::KEYWORD_HACK: return protocols::CrossTerminal::KEYWORD_HACK;
		case protocols::TokenType::KEYWORD_IF: return protocols::CrossTerminal::KEYWORD_IF;
		case protocols::TokenType::KEYWORD_LABEL: return protocols::CrossTerminal::KEYWORD_LABEL;
		case protocols::TokenType::KEYWORD_LAMBDA: return protocols::CrossTerminal::KEYWORD_LAMBDA;
		case protocols::TokenType::KEYWORD_NEW: return protocols::CrossTerminal::KEYWORD_NEW;
		case protocols::TokenType::KEYWORD_PRIVATE: return protocols::CrossTerminal::KEYWORD_PRIVATE;
		case protocols::TokenType::KEYWORD_PTR: return protocols::CrossTerminal::KEYWORD_PTR;
		case protocols::TokenType::KEYWORD_PUBLIC: return protocols::CrossTerminal::KEYWORD_PUBLIC;
		case protocols::TokenType::KEYWORD_PURE: return protocols::CrossTerminal::KEYWORD_PURE;
		case protocols::TokenType::KEYWORD_REF: return protocols::CrossTerminal::KEYWORD_REF;
		case protocols::TokenType::KEYWORD_RESTRICTED: return protocols::CrossTerminal::KEYWORD_RESTRICTED;
		case protocols::TokenType::KEYWORD_RETURN: return protocols::CrossTerminal::KEYWORD_RETURN;
		case protocols::TokenType::KEYWORD_STATIC: return protocols::CrossTerminal::KEYWORD_STATIC;
		case protocols::TokenType::KEYWORD_THIS: return protocols::CrossTerminal::KEYWORD_THIS;
		case protocols::TokenType::KEYWORD_THROW: return protocols::CrossTerminal::KEYWORD_THROW;
		case protocols::TokenType::KEYWORD_TRY: return protocols::CrossTerminal::KEYWORD_TRY;
		case protocols::TokenType::KEYWORD_TYPE: return protocols::CrossTerminal::KEYWORD_TYPE;
		case protocols::TokenType::KEYWORD_VAR: return protocols::CrossTerminal::KEYWORD_VAR;
		case protocols::TokenType::KEYWORD_WHEN: return protocols::CrossTerminal::KEYWORD_WHEN;
		case protocols::TokenType::KEYWORD_WHILE: return protocols::CrossTerminal::KEYWORD_WHILE;
		case protocols::TokenType::PRIMITIVE_SIGNED: return protocols::CrossTerminal::PRIMITIVE_SIGNED;
		case protocols::TokenType::PRIMITIVE_SIGNED_WRAPPED: return protocols::CrossTerminal::PRIMITIVE_SIGNED_WRAPPED;
		case protocols::TokenType::PRIMITIVE_UNSIGNED: return protocols::CrossTerminal::PRIMITIVE_UNSIGNED;
		case protocols::TokenType::PRIMITIVE_UNSIGNED_WRAPPED: return protocols::CrossTerminal::PRIMITIVE_UNSIGNED_WRAPPED;
		case protocols::TokenType::STRING: return protocols::CrossTerminal::STRING;
		case protocols::TokenType::SYMBOL_AMPERSAND: return protocols::CrossTerminal::SYMBOL_AMPERSAND;
		case protocols::TokenType::SYMBOL_AMPERSAND__AMPERSAND: return protocols::CrossTerminal::SYMBOL_AMPERSAND__AMPERSAND;
		case protocols::TokenType::SYMBOL_AMPERSAND__AMPERSAND__EQUAL: return protocols::CrossTerminal::SYMBOL_AMPERSAND__AMPERSAND__EQUAL;
		case protocols::TokenType::SYMBOL_AMPERSAND__EQUAL: return protocols::CrossTerminal::SYMBOL_AMPERSAND__EQUAL;
		case protocols::TokenType::SYMBOL_APETAIL: return protocols::CrossTerminal::SYMBOL_APETAIL;
		case protocols::TokenType::SYMBOL_APETAIL__APETAIL: return protocols::CrossTerminal::SYMBOL_APETAIL__APETAIL;
		case protocols::TokenType::SYMBOL_BAR: return protocols::CrossTerminal::SYMBOL_BAR;
		case protocols::TokenType::SYMBOL_BAR__BAR: return protocols::CrossTerminal::SYMBOL_BAR__BAR;
		case protocols::TokenType::SYMBOL_BAR__BAR__EQUAL: return protocols::CrossTerminal::SYMBOL_BAR__BAR__EQUAL;
		case protocols::TokenType::SYMBOL_BAR__EQUAL: return protocols::CrossTerminal::SYMBOL_BAR__EQUAL;
		case protocols::TokenType::SYMBOL_CARET: return protocols::CrossTerminal::SYMBOL_CARET;
		case protocols::TokenType::SYMBOL_CARET__EQUAL: return protocols::CrossTerminal::SYMBOL_CARET__EQUAL;
		case protocols::TokenType::SYMBOL_COLON: return protocols::CrossTerminal::SYMBOL_COLON;
		case protocols::TokenType::SYMBOL_COMMA: return protocols::CrossTerminal::SYMBOL_COMMA;
		case protocols::TokenType::SYMBOL_DOLLAR: return protocols::CrossTerminal::SYMBOL_DOLLAR;
		case protocols::TokenType::SYMBOL_DOLLAR__DOLLAR: return protocols::CrossTerminal::SYMBOL_DOLLAR__DOLLAR;
		case protocols::TokenType::SYMBOL_DOT: return protocols::CrossTerminal::SYMBOL_DOT;
		case protocols::TokenType::SYMBOL_EQUAL: return protocols::CrossTerminal::SYMBOL_EQUAL;
		case protocols::TokenType::SYMBOL_EQUAL__EQUAL: return protocols::CrossTerminal::SYMBOL_EQUAL__EQUAL;
		case protocols::TokenType::SYMBOL_EXCLAMATION_MARK: return protocols::CrossTerminal::SYMBOL_EXCLAMATION_MARK;
		case protocols::TokenType::SYMBOL_EXCLAMATION_MARK__EQUAL: return protocols::CrossTerminal::SYMBOL_EXCLAMATION_MARK__EQUAL;
		case protocols::TokenType::SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK: return protocols::CrossTerminal::SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK;
		case protocols::TokenType::SYMBOL_FORWARD_SLASH: return protocols::CrossTerminal::SYMBOL_FORWARD_SLASH;
		case protocols::TokenType::SYMBOL_FORWARD_SLASH__EQUAL: return protocols::CrossTerminal::SYMBOL_FORWARD_SLASH__EQUAL;
		case protocols::TokenType::SYMBOL_GREATER_THAN: return protocols::CrossTerminal::SYMBOL_GREATER_THAN;
		case protocols::TokenType::SYMBOL_GREATER_THAN__EQUAL: return protocols::CrossTerminal::SYMBOL_GREATER_THAN__EQUAL;
		case protocols::TokenType::SYMBOL_GREATER_THAN__GREATER_THAN: return protocols::CrossTerminal::SYMBOL_GREATER_THAN__GREATER_THAN;
		case protocols::TokenType::SYMBOL_GREATER_THAN__GREATER_THAN__EQUAL: return protocols::CrossTerminal::SYMBOL_GREATER_THAN__GREATER_THAN__EQUAL;
		case protocols::TokenType::SYMBOL_LESS_THAN: return protocols::CrossTerminal::SYMBOL_LESS_THAN;
		case protocols::TokenType::SYMBOL_LESS_THAN__EQUAL: return protocols::CrossTerminal::SYMBOL_LESS_THAN__EQUAL;
		case protocols::TokenType::SYMBOL_LESS_THAN__LESS_THAN: return protocols::CrossTerminal::SYMBOL_LESS_THAN__LESS_THAN;
		case protocols::TokenType::SYMBOL_LESS_THAN__LESS_THAN__EQUAL: return protocols::CrossTerminal::SYMBOL_LESS_THAN__LESS_THAN__EQUAL;
		case protocols::TokenType::SYMBOL_MINUS: return protocols::CrossTerminal::SYMBOL_MINUS;
		case protocols::TokenType::SYMBOL_MINUS__EQUAL: return protocols::CrossTerminal::SYMBOL_MINUS__EQUAL;
		case protocols::TokenType::SYMBOL_MINUS__GREATER_THAN: return protocols::CrossTerminal::SYMBOL_MINUS__GREATER_THAN;
		case protocols::TokenType::SYMBOL_MINUS__MINUS: return protocols::CrossTerminal::SYMBOL_MINUS__MINUS;
		case protocols::TokenType::SYMBOL_PERCENT: return protocols::CrossTerminal::SYMBOL_PERCENT;
		case protocols::TokenType::SYMBOL_PERCENT__EQUAL: return protocols::CrossTerminal::SYMBOL_PERCENT__EQUAL;
		case protocols::TokenType::SYMBOL_PLUS: return protocols::CrossTerminal::SYMBOL_PLUS;
		case protocols::TokenType::SYMBOL_PLUS__EQUAL: return protocols::CrossTerminal::SYMBOL_PLUS__EQUAL;
		case protocols::TokenType::SYMBOL_PLUS__PLUS: return protocols::CrossTerminal::SYMBOL_PLUS__PLUS;
		case protocols::TokenType::SYMBOL_SEMICOLON: return protocols::CrossTerminal::SYMBOL_SEMICOLON;
		case protocols::TokenType::SYMBOL_STAR: return protocols::CrossTerminal::SYMBOL_STAR;
		case protocols::TokenType::SYMBOL_STAR__EQUAL: return protocols::CrossTerminal::SYMBOL_STAR__EQUAL;
		case protocols::TokenType::SYMBOL_TILDE: return protocols::CrossTerminal::SYMBOL_TILDE;
		case protocols::TokenType::UNIDENTIFIED: return protocols::CrossTerminal::UNIDENTIFIED;
		default: return protocols::CrossTerminal::UNIDENTIFIED;
	}
}

}}
