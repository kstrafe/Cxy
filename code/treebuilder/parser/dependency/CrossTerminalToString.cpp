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
#include "CrossTerminalToString.hpp"


namespace tul
{
	namespace treebuilder
	{
		namespace parser
		{
			namespace dependency
			{
				std::string CrossTerminalToString::convertToString(protocols::CrossTerminal cross_terminal)
				{
					switch (cross_terminal)
					{
						case protocols::CrossTerminal::ACCESS_SPECIFIER: return "ACCESS_SPECIFIER";
						case protocols::CrossTerminal::ADDITIVE_EXPRESSION: return "ADDITIVE_EXPRESSION";
						case protocols::CrossTerminal::AND_EXPRESSION: return "AND_EXPRESSION";
						case protocols::CrossTerminal::ARGUMENT: return "ARGUMENT";
						case protocols::CrossTerminal::ARGUMENT_LIST: return "ARGUMENT_LIST";
						case protocols::CrossTerminal::ASSIGNMENT: return "ASSIGNMENT";
						case protocols::CrossTerminal::ATTRIBUTE_LIST: return "ATTRIBUTE_LIST";
						case protocols::CrossTerminal::ATTRIBUTE_LIST_AFTER_CONST: return "ATTRIBUTE_LIST_AFTER_CONST";
						case protocols::CrossTerminal::BASIC_TYPE: return "BASIC_TYPE";
						case protocols::CrossTerminal::BITWISE_AND_EXPRESSION: return "BITWISE_AND_EXPRESSION";
						case protocols::CrossTerminal::BITWISE_OR_EXPRESSION: return "BITWISE_OR_EXPRESSION";
						case protocols::CrossTerminal::BITWISE_XOR_EXPRESSION: return "BITWISE_XOR_EXPRESSION";
						case protocols::CrossTerminal::CALL: return "CALL";
						case protocols::CrossTerminal::CODE_BLOCK: return "CODE_BLOCK";
						case protocols::CrossTerminal::COMMA_SEPARATED_LIST: return "COMMA_SEPARATED_LIST";
						case protocols::CrossTerminal::COMMA_SEPARATED_LIST_ITEM: return "COMMA_SEPARATED_LIST_ITEM";
						case protocols::CrossTerminal::COMMA_SEPARATED_TYPE_AND_NAME_LIST: return "COMMA_SEPARATED_TYPE_AND_NAME_LIST";
						case protocols::CrossTerminal::DATA_DECLARATION: return "DATA_DECLARATION";
						case protocols::CrossTerminal::DECLARATION: return "DECLARATION";
						case protocols::CrossTerminal::DECL_OR_FUNC: return "DECL_OR_FUNC";
						case protocols::CrossTerminal::DO_STATEMENT: return "DO_STATEMENT";
						case protocols::CrossTerminal::ENTER: return "ENTER";
						case protocols::CrossTerminal::EPSILONATE: return "EPSILONATE";
						case protocols::CrossTerminal::EQUALITY_EXPRESSION: return "EQUALITY_EXPRESSION";
						case protocols::CrossTerminal::EXIT: return "EXIT";
						case protocols::CrossTerminal::EXPRESSION: return "EXPRESSION";
						case protocols::CrossTerminal::FIELD_DECLARATION_LIST: return "FIELD_DECLARATION_LIST";
						case protocols::CrossTerminal::FOR_STATEMENT: return "FOR_STATEMENT";
						case protocols::CrossTerminal::FUNCTION_DEFINITION: return "FUNCTION_DEFINITION";
						case protocols::CrossTerminal::FUNCTION_LIST: return "FUNCTION_LIST";
						case protocols::CrossTerminal::FUNCTION_SIGNATURE: return "FUNCTION_SIGNATURE";
						case protocols::CrossTerminal::GOTO_STATEMENT: return "GOTO_STATEMENT";
						case protocols::CrossTerminal::IF_STATEMENT: return "IF_STATEMENT";
						case protocols::CrossTerminal::ITER_STATEMENT: return "ITER_STATEMENT";
						case protocols::CrossTerminal::LABEL_STATEMENT: return "LABEL_STATEMENT";
						case protocols::CrossTerminal::MEMBER_EXPRESSION: return "MEMBER_EXPRESSION";
						case protocols::CrossTerminal::METHOD_DECLARATION_LIST: return "METHOD_DECLARATION_LIST";
						case protocols::CrossTerminal::MULTIPLICATIVE_EXPRESSION: return "MULTIPLICATIVE_EXPRESSION";
						case protocols::CrossTerminal::NONE: return "NONE";
						case protocols::CrossTerminal::NO_SEMICOLON_STATEMENT: return "NO_SEMICOLON_STATEMENT";
						case protocols::CrossTerminal::OPTIONAL_ADDITIVE_EXPRESSION: return "OPTIONAL_ADDITIVE_EXPRESSION";
						case protocols::CrossTerminal::OPTIONAL_AND_EXPRESSION: return "OPTIONAL_AND_EXPRESSION";
						case protocols::CrossTerminal::OPTIONAL_ARGUMENT_LIST: return "OPTIONAL_ARGUMENT_LIST";
						case protocols::CrossTerminal::OPTIONAL_ASSIGNMENT: return "OPTIONAL_ASSIGNMENT";
						case protocols::CrossTerminal::OPTIONAL_ATTRIBUTE_LIST: return "OPTIONAL_ATTRIBUTE_LIST";
						case protocols::CrossTerminal::OPTIONAL_BITWISE_AND_EXPRESSION: return "OPTIONAL_BITWISE_AND_EXPRESSION";
						case protocols::CrossTerminal::OPTIONAL_BITWISE_OR_EXPRESSION: return "OPTIONAL_BITWISE_OR_EXPRESSION";
						case protocols::CrossTerminal::OPTIONAL_BITWISE_XOR_EXPRESSION: return "OPTIONAL_BITWISE_XOR_EXPRESSION";
						case protocols::CrossTerminal::OPTIONAL_CALL: return "OPTIONAL_CALL";
						case protocols::CrossTerminal::OPTIONAL_EQUALITY_EXPRESSION: return "OPTIONAL_EQUALITY_EXPRESSION";
						case protocols::CrossTerminal::OPTIONAL_EQUALS: return "OPTIONAL_EQUALS";
						case protocols::CrossTerminal::OPTIONAL_MEMBER_EXPRESSION: return "OPTIONAL_MEMBER_EXPRESSION";
						case protocols::CrossTerminal::OPTIONAL_MULTIPLICATIVE_EXPRESSION: return "OPTIONAL_MULTIPLICATIVE_EXPRESSION";
						case protocols::CrossTerminal::OPTIONAL_OR_EXPRESSION: return "OPTIONAL_OR_EXPRESSION";
						case protocols::CrossTerminal::OPTIONAL_RELATIONAL_EXPRESSION: return "OPTIONAL_RELATIONAL_EXPRESSION";
						case protocols::CrossTerminal::OPTIONAL_STRING_PLUS: return "OPTIONAL_STRING_PLUS";
						case protocols::CrossTerminal::OR_EXPRESSION: return "OR_EXPRESSION";
						case protocols::CrossTerminal::RELATIONAL_EXPRESSION: return "RELATIONAL_EXPRESSION";
						case protocols::CrossTerminal::RELATIONAL_OPERATOR: return "RELATIONAL_OPERATOR";
						case protocols::CrossTerminal::RESOURCE: return "RESOURCE";
						case protocols::CrossTerminal::STATEMENT: return "STATEMENT";
						case protocols::CrossTerminal::STATEMENT_LIST: return "STATEMENT_LIST";
						case protocols::CrossTerminal::SUBROUTINE: return "SUBROUTINE";
						case protocols::CrossTerminal::SUBROUTINE_LIST: return "SUBROUTINE_LIST";
						case protocols::CrossTerminal::TYPE_PREFIX: return "TYPE_PREFIX";
						case protocols::CrossTerminal::TYPE_PREFIX_AFTER_CONST: return "TYPE_PREFIX_AFTER_CONST";
						case protocols::CrossTerminal::TYPE_PREFIX_AFTER_REF: return "TYPE_PREFIX_AFTER_REF";
						case protocols::CrossTerminal::TYPE_PREFIX_AFTER_REF_CONST: return "TYPE_PREFIX_AFTER_REF_CONST";
						case protocols::CrossTerminal::UNARY_EXPRESSION: return "UNARY_EXPRESSION";
						case protocols::CrossTerminal::WHILE_STATEMENT: return "WHILE_STATEMENT";
						case protocols::CrossTerminal::GROUPER_LEFT_BRACE: return "GROUPER_LEFT_BRACE";
						case protocols::CrossTerminal::GROUPER_LEFT_BRACKET: return "GROUPER_LEFT_BRACKET";
						case protocols::CrossTerminal::GROUPER_LEFT_PARENTHESIS: return "GROUPER_LEFT_PARENTHESIS";
						case protocols::CrossTerminal::GROUPER_RIGHT_BRACE: return "GROUPER_RIGHT_BRACE";
						case protocols::CrossTerminal::GROUPER_RIGHT_BRACKET: return "GROUPER_RIGHT_BRACKET";
						case protocols::CrossTerminal::GROUPER_RIGHT_PARENTHESIS: return "GROUPER_RIGHT_PARENTHESIS";
						case protocols::CrossTerminal::IDENTIFIER_CLASS: return "IDENTIFIER_CLASS";
						case protocols::CrossTerminal::IDENTIFIER_ENUMERATION: return "IDENTIFIER_ENUMERATION";
						case protocols::CrossTerminal::IDENTIFIER_PACKAGE: return "IDENTIFIER_PACKAGE";
						case protocols::CrossTerminal::IDENTIFIER_SUBROUTINE: return "IDENTIFIER_SUBROUTINE";
						case protocols::CrossTerminal::IDENTIFIER_VARIABLE: return "IDENTIFIER_VARIABLE";
						case protocols::CrossTerminal::INTEGER_LITERAL: return "INTEGER_LITERAL";
						case protocols::CrossTerminal::KEYWORD_ASSEMBLY: return "KEYWORD_ASSEMBLY";
						case protocols::CrossTerminal::KEYWORD_CONST: return "KEYWORD_CONST";
						case protocols::CrossTerminal::KEYWORD_DO: return "KEYWORD_DO";
						case protocols::CrossTerminal::KEYWORD_FOR: return "KEYWORD_FOR";
						case protocols::CrossTerminal::KEYWORD_GOTO: return "KEYWORD_GOTO";
						case protocols::CrossTerminal::KEYWORD_IF: return "KEYWORD_IF";
						case protocols::CrossTerminal::KEYWORD_LABEL: return "KEYWORD_LABEL";
						case protocols::CrossTerminal::KEYWORD_PRIVATE: return "KEYWORD_PRIVATE";
						case protocols::CrossTerminal::KEYWORD_PTR: return "KEYWORD_PTR";
						case protocols::CrossTerminal::KEYWORD_PUBLIC: return "KEYWORD_PUBLIC";
						case protocols::CrossTerminal::KEYWORD_PURE: return "KEYWORD_PURE";
						case protocols::CrossTerminal::KEYWORD_REF: return "KEYWORD_REF";
						case protocols::CrossTerminal::KEYWORD_RESTRICTED: return "KEYWORD_RESTRICTED";
						case protocols::CrossTerminal::KEYWORD_WHILE: return "KEYWORD_WHILE";
						case protocols::CrossTerminal::PRIMITIVE_SIGNED: return "PRIMITIVE_SIGNED";
						case protocols::CrossTerminal::PRIMITIVE_UNSIGNED: return "PRIMITIVE_UNSIGNED";
						case protocols::CrossTerminal::STRING: return "STRING";
						case protocols::CrossTerminal::SYMBOL_AMPERSAND: return "SYMBOL_AMPERSAND";
						case protocols::CrossTerminal::SYMBOL_AMPERSAND__AMPERSAND: return "SYMBOL_AMPERSAND__AMPERSAND";
						case protocols::CrossTerminal::SYMBOL_APETAIL: return "SYMBOL_APETAIL";
						case protocols::CrossTerminal::SYMBOL_APETAIL__APETAIL: return "SYMBOL_APETAIL__APETAIL";
						case protocols::CrossTerminal::SYMBOL_BACKWARD_SLASH: return "SYMBOL_BACKWARD_SLASH";
						case protocols::CrossTerminal::SYMBOL_BAR: return "SYMBOL_BAR";
						case protocols::CrossTerminal::SYMBOL_BAR__BAR: return "SYMBOL_BAR__BAR";
						case protocols::CrossTerminal::SYMBOL_CARET: return "SYMBOL_CARET";
						case protocols::CrossTerminal::SYMBOL_COLON: return "SYMBOL_COLON";
						case protocols::CrossTerminal::SYMBOL_COLON__COLON: return "SYMBOL_COLON__COLON";
						case protocols::CrossTerminal::SYMBOL_COMMA: return "SYMBOL_COMMA";
						case protocols::CrossTerminal::SYMBOL_DOLLAR: return "SYMBOL_DOLLAR";
						case protocols::CrossTerminal::SYMBOL_DOLLAR__DOLLAR: return "SYMBOL_DOLLAR__DOLLAR";
						case protocols::CrossTerminal::SYMBOL_DOT: return "SYMBOL_DOT";
						case protocols::CrossTerminal::SYMBOL_DOT__DOT: return "SYMBOL_DOT__DOT";
						case protocols::CrossTerminal::SYMBOL_EQUAL: return "SYMBOL_EQUAL";
						case protocols::CrossTerminal::SYMBOL_EQUAL__EQUAL: return "SYMBOL_EQUAL__EQUAL";
						case protocols::CrossTerminal::SYMBOL_EXCLAMATION_MARK: return "SYMBOL_EXCLAMATION_MARK";
						case protocols::CrossTerminal::SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK: return "SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK";
						case protocols::CrossTerminal::SYMBOL_FORWARD_SLASH: return "SYMBOL_FORWARD_SLASH";
						case protocols::CrossTerminal::SYMBOL_GREATER_THAN: return "SYMBOL_GREATER_THAN";
						case protocols::CrossTerminal::SYMBOL_GREATER_THAN__EQUAL: return "SYMBOL_GREATER_THAN__EQUAL";
						case protocols::CrossTerminal::SYMBOL_GREATER_THAN__GREATER_THAN: return "SYMBOL_GREATER_THAN__GREATER_THAN";
						case protocols::CrossTerminal::SYMBOL_LESS_THAN: return "SYMBOL_LESS_THAN";
						case protocols::CrossTerminal::SYMBOL_LESS_THAN__EQUAL: return "SYMBOL_LESS_THAN__EQUAL";
						case protocols::CrossTerminal::SYMBOL_LESS_THAN__LESS_THAN: return "SYMBOL_LESS_THAN__LESS_THAN";
						case protocols::CrossTerminal::SYMBOL_MINUS: return "SYMBOL_MINUS";
						case protocols::CrossTerminal::SYMBOL_MINUS__MINUS: return "SYMBOL_MINUS__MINUS";
						case protocols::CrossTerminal::SYMBOL_PLUS: return "SYMBOL_PLUS";
						case protocols::CrossTerminal::SYMBOL_PLUS__PLUS: return "SYMBOL_PLUS__PLUS";
						case protocols::CrossTerminal::SYMBOL_SEMICOLON: return "SYMBOL_SEMICOLON";
						case protocols::CrossTerminal::SYMBOL_STAR: return "SYMBOL_STAR";
						case protocols::CrossTerminal::SYMBOL_STAR__STAR: return "SYMBOL_STAR__STAR";
						case protocols::CrossTerminal::UNIDENTIFIED: return "UNIDENTIFIED";
						default: return "";
					}
				}
			}
		}
	}
}