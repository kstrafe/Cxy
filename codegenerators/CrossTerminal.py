# -*- coding: utf-8 -*-
# Copyright © 2015 Kevin Robert Stravers
"""
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
"""

LICENSE_STRING = '''// Copyright © 2015 Kevin Robert Stravers
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
'''

import dependency.ParserTableGenerator as ParserTableGenerator
import dependency.NamespaceGenerator
import dependency.Prepend

# Take a list of cross-terminals and create the following files.
# Search for the word `file` in this code to find out where the folders get
# written to..
# Note: project root is the folder where readme.md is located (the root)

# These are the productions of the grammar.
# Any 'KEYWORD_' gets turned into an actual keyword recognizer (KeywordMatcher).
# The same occurs with 'SYMBOL_' (SymbolMatcher).
# Empty productions '[]' are epsilon productions.
# Generates a .inc file containing all lookahead rules.
productions = {
################################################################################ 1
	'ENTER': [
		['ALIAS_STATEMENT', 'SYMBOL_SEMICOLON__PRUNE', 'ENTER'],
		['DATA_DECLARATION', 'SYMBOL_SEMICOLON__PRUNE', 'ENTER'],
		['FUNCTION_DEFINITION', 'ENTER'],
		['GRANT_DEFINITION', 'ENTER'],
		[]
	],
################################################################################ 2
	'ALIAS_STATEMENT': [
		['KEYWORD_ALIAS', 'ALIAS_TRAIL']
	],
	'ALIAS_TRAIL': [
		['IDENTIFIER_CLASS', 'SYMBOL_EQUAL', 'TYPE'],
		['IDENTIFIER_PACKAGE', 'SYMBOL_EQUAL', 'IDENTIFIER_PACKAGE'],
	],
	'DATA_DECLARATION': [
		['TYPE_NO_FUNCTION', 'DATA_NAMES', 'OPTIONAL_ASSIGNMENT', 'OPTIONAL_DATA_DECLARATION'],
	],
	'FUNCTION_DEFINITION': [
		['FUNCTION_SIGNATURE', 'FUNCTION_NAMES', 'GROUPER_LEFT_BRACE', 'STATEMENT_LIST', 'GROUPER_RIGHT_BRACE']
	],
	'GRANT_DEFINITION': [
		['KEYWORD_GRANT', 'IDENTIFIER_CLASS', 'GROUPER_LEFT_BRACE', 'GRANT_LIST', 'GROUPER_RIGHT_BRACE']
	],
################################################################################ 3
# Naming subsytem
################################################################################ 3
	'DATA_NAMES': [
		['IDENTIFIER_VARIABLE'],
		['IDENTIFIER_PACKAGE']
	],
	'FUNCTION_NAMES': [
		['IDENTIFIER_SUBROUTINE'],
		['IDENTIFIER_PACKAGE'],
		['SYMBOL_PLUS'],
		['SYMBOL_MINUS']
	],
################################################################################ 3
# End Naming subsytem
################################################################################ 3
################################################################################ 3
# Type subsystem
# External to subsystem: OR_EXPRESSION, ARGUMENT_LIST
# Defines types: ref ptr ptr const ptr const [x * x, ptr ([3, 8u] out:)]
################################################################################
	'TYPE': [
		['ARRAY'],
		['BASIC_TYPE'],
		['FUNCTION_SIGNATURE'],
		['KEYWORD_CONST', 'TYPE_AFTER_CONST'],
		['KEYWORD_PTR', 'TYPE_AFTER_PTR'],
		['KEYWORD_REF', 'TYPE_AFTER_REF'],
	],
	'TYPE_NO_FUNCTION': [
		['ARRAY'],
		['BASIC_TYPE'],
		['KEYWORD_CONST', 'TYPE_AFTER_CONST'],
		['KEYWORD_PTR', 'TYPE_AFTER_PTR'],
		['KEYWORD_REF', 'TYPE_AFTER_REF'],
	],
################################################################################ 4
	'ARRAY': [
		['GROUPER_LEFT_BRACKET', 'INTEGER_LITERAL', 'SYMBOL_COMMA__PRUNE', 'TYPE', 'GROUPER_RIGHT_BRACKET']
	],
	'BASIC_TYPE': [
		['IDENTIFIER_CLASS', 'OPTIONAL_TEMPLATE'],
		['IDENTIFIER_PACKAGE', 'SYMBOL_DOT__PRUNE', 'IDENTIFIER_CLASS', 'OPTIONAL_TEMPLATE'],
		['KEYWORD_DOUBLE'],
		['KEYWORD_FLOAT'],
		['PRIMITIVE_SIGNED'],
		['PRIMITIVE_UNSIGNED'],
	],
	'TYPE_AFTER_CONST': [
		['ARRAY'],
		['BASIC_TYPE'],
		['KEYWORD_PTR', 'TYPE'],
	],
	'TYPE_AFTER_PTR': [
		['ARRAY'],
		['BASIC_TYPE'],
		['FUNCTION_SIGNATURE'],
		['KEYWORD_CONST', 'TYPE_AFTER_CONST'],
		['KEYWORD_PTR', 'TYPE_AFTER_PTR'],
		['KEYWORD_REF', 'TYPE_AFTER_REF']
	],
	'TYPE_AFTER_REF': [
		['ARRAY'],
		['BASIC_TYPE'],
		['FUNCTION_SIGNATURE'],
		['KEYWORD_CONST', 'TYPE_AFTER_REF_CONST'],
		['KEYWORD_PTR', 'TYPE'],
	],
################################################################################ 5
	'OPTIONAL_TEMPLATE': [
		['GROUPER_LEFT_BRACKET', 'TEMPLATE_LIST', 'GROUPER_RIGHT_BRACKET'],
		[]
	],
	'TYPE_AFTER_REF_CONST': [
		['BASIC_TYPE'],
		['KEYWORD_PTR', 'TYPE'],
	],
	'FUNCTION_SIGNATURE': [
		['GROUPER_LEFT_PARENTHESIS', 'ARGUMENT_LIST', 'SYMBOL_COLON__PRUNE', 'ARGUMENT_LIST', 'OPTIONAL_ATTRIBUTE_LIST', 'GROUPER_RIGHT_PARENTHESIS'],
		],
################################################################################ 6
	'TEMPLATE_LIST': [
		['IDENTIFIER_CLASS', 'SYMBOL_COLON__PRUNE', 'TYPE', 'OPTIONAL_TEMPLATE_LIST'],
		['SYMBOL_COLON__PRUNE', 'TYPE', 'OPTIONAL_TEMPLATE_LIST'],
	],
	'OPTIONAL_ATTRIBUTE_LIST': [
		['SYMBOL_COLON__PRUNE', 'ATTRIBUTE_LIST'],
		[]
	],
################################################################################ 7
	'OPTIONAL_TEMPLATE_LIST': [
		['IDENTIFIER_CLASS', 'SYMBOL_COLON__PRUNE', 'TYPE', 'OPTIONAL_TEMPLATE_LIST'],
		['SYMBOL_COLON__PRUNE', 'TYPE', 'OPTIONAL_TEMPLATE_LIST'],
		[]
	],
	'ATTRIBUTE_LIST': [
		['KEYWORD_CONST', 'ATTRIBUTE_LIST_AFTER_CONST'],
		['KEYWORD_PURE'],
		[]
	],
################################################################################ 8
	'ATTRIBUTE_LIST_AFTER_CONST': [
		['KEYWORD_PURE'],
		[]
	],
################################################################################
# End of Type subsystem
################################################################################ 9
	'ARGUMENT_LIST': [
		['ARGUMENT', 'OPTIONAL_ARGUMENT_LIST'],
		[],
	],
	'OPTIONAL_ASSIGNMENT': [
		['GROUPER_LEFT_PARENTHESIS', 'PARAMETER_LIST', 'GROUPER_RIGHT_PARENTHESIS'],
		['SYMBOL_EQUAL', 'EXPRESSION_EXPRESSION'],
		[]
	],
	'OPTIONAL_DATA_DECLARATION': [
		['SYMBOL_COMMA__PRUNE', 'DATA_NAMES', 'OPTIONAL_ASSIGNMENT', 'OPTIONAL_DATA_DECLARATION'],
		[],
	],
	'GRANT_LIST': [
		['TYPE_NO_FUNCTION', 'DATA_NAMES', 'OPTIONAL_DATA_NAME_LIST', 'SYMBOL_SEMICOLON__PRUNE', 'OPTIONAL_GRANT_LIST'],
		['FUNCTION_SIGNATURE', 'FUNCTION_NAMES', 'OPTIONAL_FUNCTION_NAME_LIST', 'SYMBOL_SEMICOLON__PRUNE', 'OPTIONAL_GRANT_LIST']
	],
################################################################################ 10
	'STATEMENT_LIST': [
		['NO_SEMICOLON_STATEMENT', 'STATEMENT_LIST'],
		['STATEMENT', 'SYMBOL_SEMICOLON__PRUNE', 'STATEMENT_LIST'],
		[]
	],
	'NO_SEMICOLON_STATEMENT': [
		['CODE_BLOCK'],
		['DEFER_STATEMENT'],
		['DO_STATEMENT'],
		['FOR_STATEMENT'],
		['IF_STATEMENT'],
		['STATIC_IF_STATEMENT'],
		['WHILE_STATEMENT'],
	],
	'STATEMENT': [
		['ALIAS_STATEMENT'],
		['DATA_DECLARATION_STATEMENT'],
		['EXPRESSION_EXPRESSION'],
		['GOTO_STATEMENT'],
		['HACK_STATEMENT'],
		['ITER_STATEMENT'],
		['LABEL_STATEMENT'],
		['RETURN_STATEMENT'],
	],
	'PARAMETER_LIST': [
		['DATA_NAMES', 'SYMBOL_COLON__PRUNE', 'EXPRESSION_EXPRESSION', 'OPTIONAL_PARAMETER_LIST'],
		['SYMBOL_COLON__PRUNE', 'EXPRESSION_EXPRESSION', 'OPTIONAL_PARAMETER_LIST'],
		[]
	],
	'EXPRESSION_EXPRESSION': [
		['ASSIGNMENT_EXPRESSION'],
	],
	'OPTIONAL_DATA_NAME_LIST': [
		['SYMBOL_COMMA__PRUNE', 'DATA_NAMES', 'OPTIONAL_DATA_NAME_LIST'],
		[]
	],
	'OPTIONAL_FUNCTION_NAME_LIST': [
		['SYMBOL_COMMA__PRUNE', 'FUNCTION_NAMES', 'OPTIONAL_FUNCTION_NAME_LIST'],
		[]
	],
	'OPTIONAL_GRANT_LIST': [
		['GRANT_LIST'],
		[]
	],
################################################################################ 11
	'ARGUMENT': [
		['TYPE', 'DATA_NAMES', 'OPTIONAL_ASSIGNMENT'],
	],
	'OPTIONAL_ARGUMENT_LIST': [
		['SYMBOL_COMMA__PRUNE', 'ARGUMENT_LIST_AFTER_FIRST'],
		[]
	],
	'DEFER_STATEMENT': [
		['KEYWORD_DEFER', 'SINGLE_STATEMENT_OR_CODE_BLOCK']
	],
	'DO_STATEMENT': [
		['KEYWORD_DO', 'GROUPER_LEFT_PARENTHESIS', 'EXPRESSION_EXPRESSION', 'GROUPER_RIGHT_PARENTHESIS', 'SINGLE_STATEMENT_OR_CODE_BLOCK']
	],
	'FOR_STATEMENT': [
		['KEYWORD_FOR', 'GROUPER_LEFT_PARENTHESIS', 'DATA_DECLARATION', 'SYMBOL_SEMICOLON__PRUNE', 'EXPRESSION_EXPRESSION', 'SYMBOL_SEMICOLON__PRUNE', 'STATEMENT_LIST', 'GROUPER_RIGHT_PARENTHESIS', 'SINGLE_STATEMENT_OR_CODE_BLOCK']
	],
	'IF_STATEMENT': [
		['KEYWORD_IF', 'GROUPER_LEFT_PARENTHESIS', 'EXPRESSION_EXPRESSION', 'GROUPER_RIGHT_PARENTHESIS', 'SINGLE_STATEMENT_OR_CODE_BLOCK', 'ELSE_STATEMENT']
	],
	'STATIC_ELSE_STATEMENT': [
		['KEYWORD_STATIC', 'KEYWORD_ELSE', 'SINGLE_STATEMENT_OR_CODE_BLOCK'],
		[]
	],
	'STATIC_IF_STATEMENT': [
		['KEYWORD_STATIC', 'KEYWORD_IF', 'GROUPER_LEFT_PARENTHESIS', 'EXPRESSION_EXPRESSION', 'GROUPER_RIGHT_PARENTHESIS', 'SINGLE_STATEMENT_OR_CODE_BLOCK', 'STATIC_ELSE_STATEMENT']
	],
	'WHILE_STATEMENT': [
		['KEYWORD_WHILE', 'GROUPER_LEFT_PARENTHESIS', 'EXPRESSION_EXPRESSION', 'GROUPER_RIGHT_PARENTHESIS', 'SINGLE_STATEMENT_OR_CODE_BLOCK']
	],
	'CODE_BLOCK': [
		['GROUPER_LEFT_BRACE', 'STATEMENT_LIST', 'GROUPER_RIGHT_BRACE'],
	],
	'DATA_DECLARATION_STATEMENT': [
		['KEYWORD_VAR', 'DATA_DECLARATION_TYPE'],
	],
	'DATA_DECLARATION_TYPE': [
		['GROUPER_LEFT_BRACE', 'DATA_DECLARATION_LIST', 'GROUPER_RIGHT_BRACE'],
		['TYPE', 'DATA_NAMES', 'OPTIONAL_ASSIGNMENT', 'OPTIONAL_DATA_DECLARATION'],
	],
	'DATA_DECLARATION_LIST': [
		['TYPE', 'DATA_NAMES', 'OPTIONAL_ASSIGNMENT', 'SYMBOL_SEMICOLON__PRUNE', 'DATA_DECLARATION_LIST'],
		[]
	],
	'GOTO_STATEMENT': [
		['KEYWORD_GOTO', 'DATA_NAMES']
	],
	'HACK_STATEMENT': [
		['KEYWORD_HACK', 'GROUPER_LEFT_PARENTHESIS', 'PARAMETER_LIST', 'GROUPER_RIGHT_PARENTHESIS']
	],
	'ITER_STATEMENT': [
		['SYMBOL_PLUS__PLUS', 'EXPRESSION_EXPRESSION'],
		['SYMBOL_MINUS__MINUS', 'EXPRESSION_EXPRESSION'],
	],
	'LABEL_STATEMENT': [
		['KEYWORD_LABEL', 'DATA_NAMES']
	],
	'RETURN_STATEMENT': [
		['KEYWORD_RETURN', 'PARAMETER_LIST'],
	],
	'OPTIONAL_PARAMETER_LIST': [
		['SYMBOL_COMMA__PRUNE', 'PARAMETER_LIST'],
		[]
	],
	'ASSIGNMENT_EXPRESSION': [
		['OR_EXPRESSION', 'OPTIONAL_ASSIGNMENT_EXPRESSION']
	],
################################################################################ 12
	'ARGUMENT_LIST_AFTER_FIRST': [
		['TYPE', 'DATA_NAMES', 'OPTIONAL_ASSIGNMENT', 'OPTIONAL_ARGUMENT_LIST', 'OPTIONAL_ARGUMENT_LIST'],
		#['IDENTIFIER_VARIABLE', 'OPTIONAL_ARGUMENT_LIST', 'OPTIONAL_ARGUMENT_LIST'],
		[],
	],
	'SINGLE_STATEMENT_OR_CODE_BLOCK': [
		['STATEMENT', 'SYMBOL_SEMICOLON__PRUNE'],
		['NO_SEMICOLON_STATEMENT'],
	],
	'ELSE_STATEMENT': [
		['KEYWORD_ELSE', 'SINGLE_STATEMENT_OR_CODE_BLOCK'],
		[]
	],
################################################################################ 13
# EXPRESSION BLOCK.
# Contains all valid expressions in the language. These are sums, == compares...
# a + b * c - d
################################################################################
################################################################################
	'OR_EXPRESSION': [
		['AND_EXPRESSION', 'OPTIONAL_OR_EXPRESSION'],
	],
	'OPTIONAL_ASSIGNMENT_EXPRESSION': [
		['SYMBOL_EQUAL', 'OR_EXPRESSION'],
		['SYMBOL_FORWARD_SLASH__EQUAL', 'OR_EXPRESSION'],
		['SYMBOL_MINUS__EQUAL', 'OR_EXPRESSION'],
		['SYMBOL_PLUS__EQUAL', 'OR_EXPRESSION'],
		['SYMBOL_STAR__EQUAL', 'OR_EXPRESSION'],
		[]
	],
################################################################################ 14
	'AND_EXPRESSION': [
		['BITWISE_OR_EXPRESSION', 'OPTIONAL_AND_EXPRESSION'],
	],
	'OPTIONAL_OR_EXPRESSION': [
		['SYMBOL_BAR__BAR', 'OR_EXPRESSION'],
		[]
	],
################################################################################ 15
	'BITWISE_OR_EXPRESSION': [
		['BITWISE_XOR_EXPRESSION', 'OPTIONAL_BITWISE_OR_EXPRESSION'],
	],
	'OPTIONAL_AND_EXPRESSION': [
		['SYMBOL_AMPERSAND__AMPERSAND', 'AND_EXPRESSION'],
		[]
	],
################################################################################ 16
	'BITWISE_XOR_EXPRESSION': [
		['BITWISE_AND_EXPRESSION', 'OPTIONAL_BITWISE_XOR_EXPRESSION'],
	],
	'OPTIONAL_BITWISE_OR_EXPRESSION': [
		['SYMBOL_BAR', 'BITWISE_OR_EXPRESSION'],
		[]
	],
################################################################################ 17
	'BITWISE_AND_EXPRESSION': [
		['EQUALITY_EXPRESSION', 'OPTIONAL_BITWISE_AND_EXPRESSION'],
	],
	'OPTIONAL_BITWISE_XOR_EXPRESSION': [
		['SYMBOL_CARET', 'BITWISE_XOR_EXPRESSION'],
		[]
	],
################################################################################ 18
	'EQUALITY_EXPRESSION': [
		['RELATIONAL_EXPRESSION', 'OPTIONAL_EQUALITY_EXPRESSION'],
	],
	'OPTIONAL_BITWISE_AND_EXPRESSION': [
		['SYMBOL_AMPERSAND', 'BITWISE_AND_EXPRESSION'],
		[]
	],
################################################################################ 19
	'RELATIONAL_EXPRESSION': [
		['ADDITIVE_EXPRESSION', 'OPTIONAL_RELATIONAL_EXPRESSION'],
	],
	'OPTIONAL_EQUALITY_EXPRESSION': [
		['SYMBOL_EQUAL__EQUAL', 'EQUALITY_EXPRESSION'],
		[]
	],
################################################################################ 20
	'ADDITIVE_EXPRESSION': [
		['MULTIPLICATIVE_EXPRESSION', 'OPTIONAL_ADDITIVE_EXPRESSION'],
	],
	'OPTIONAL_RELATIONAL_EXPRESSION': [
		['RELATIONAL_OPERATOR', 'EQUALITY_EXPRESSION'],
		[]
	],
################################################################################ 21
	'MULTIPLICATIVE_EXPRESSION': [
		['CAST_EXPRESSION', 'OPTIONAL_MULTIPLICATIVE_EXPRESSION'],
	],
	'OPTIONAL_ADDITIVE_EXPRESSION': [
		['SYMBOL_MINUS', 'ADDITIVE_EXPRESSION'],
		['SYMBOL_PLUS', 'ADDITIVE_EXPRESSION'],
		[]
	],
	'RELATIONAL_OPERATOR': [
		['SYMBOL_LESS_THAN'],
		['SYMBOL_GREATER_THAN'],
		['SYMBOL_LESS_THAN__EQUAL'],
		['SYMBOL_GREATER_THAN__EQUAL'],
	],
################################################################################ 22
	'OPTIONAL_MULTIPLICATIVE_EXPRESSION': [
		['SYMBOL_FORWARD_SLASH', 'MULTIPLICATIVE_EXPRESSION'],
		['SYMBOL_STAR', 'MULTIPLICATIVE_EXPRESSION'],
		[]
	],
	'CAST_EXPRESSION': [
		['KEYWORD_CAST', 'GROUPER_LEFT_BRACKET', 'TYPE', 'GROUPER_RIGHT_BRACKET', 'GROUPER_LEFT_PARENTHESIS', 'EXPRESSION_EXPRESSION', 'GROUPER_RIGHT_PARENTHESIS'],
		['NEW_EXPRESSION'],
	],
################################################################################ 23
	'NEW_EXPRESSION': [
		['KEYWORD_NEW', 'GROUPER_LEFT_BRACKET', 'TYPE', 'GROUPER_RIGHT_BRACKET', 'GROUPER_LEFT_PARENTHESIS', 'EXPRESSION_EXPRESSION', 'GROUPER_RIGHT_PARENTHESIS'],
		['UNARY_EXPRESSION'],
	],
################################################################################ 23.1
	'UNARY_EXPRESSION': [
		['MEMBER_EXPRESSION'],
		['UNARY_OPERATOR', 'CAST_EXPRESSION'],
	],
################################################################################ 24
	'MEMBER_EXPRESSION': [
		['IDENTIFIER_CLASS', 'CLASS_MEMBER_EXPRESSION'],
		['IDENTIFIER_ENUMERATION', 'ENUMERATION_MEMBER_EXPRESSION'],
		['IDENTIFIER_PACKAGE', 'OPTIONAL_MEMBER_EXPRESSION'],
		['IDENTIFIER_VARIABLE', 'OPTIONAL_MEMBER_EXPRESSION'],
		['IDENTIFIER_SUBROUTINE', 'OPTIONAL_MEMBER_EXPRESSION'],
		['RESOURCE', 'OPTIONAL_MEMBER_EXPRESSION'],
	],
	'UNARY_OPERATOR': [
		['SYMBOL_APETAIL'],
		['SYMBOL_APETAIL__APETAIL'],
		['SYMBOL_DOLLAR'],
		['SYMBOL_DOLLAR__DOLLAR'],
		['SYMBOL_EXCLAMATION_MARK'],
		['SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK'],
		['SYMBOL_MINUS'],
	],
################################################################################ 25
	'CLASS_MEMBER_EXPRESSION': [
		['GROUPER_LEFT_PARENTHESIS', 'PARAMETER_LIST', 'GROUPER_RIGHT_PARENTHESIS'],
		['SYMBOL_DOT__PRUNE', 'CLASS_MEMBER', 'OPTIONAL_MEMBER_EXPRESSION'],
	],
	'ENUMERATION_MEMBER_EXPRESSION': [
		['SYMBOL_DOT__PRUNE', 'IDENTIFIER_VARIABLE'],
		[]
	],
	'RESOURCE': [
		['INTEGER_LITERAL'],
		['GROUPER_LEFT_PARENTHESIS', 'EXPRESSION_EXPRESSION', 'GROUPER_RIGHT_PARENTHESIS'],
		['STRING'],
		['WHEN_EXPRESSION'],
		['KEYWORD_LAMBDA', 'OPTIONAL_CAPTURE_LIST', 'FUNCTION_SIGNATURE', 'CODE_BLOCK']
	],
	'OPTIONAL_MEMBER_EXPRESSION': [
		['ARRAY_ACCESS_EXPRESSION'],
		['GROUPER_LEFT_PARENTHESIS', 'PARAMETER_LIST', 'GROUPER_RIGHT_PARENTHESIS', 'OPTIONAL_EXTRACTOR_EXPRESSION'],
		['SYMBOL_DOT__PRUNE', 'MEMBER_EXPRESSION'],
		[]
	],
################################################################################ 26
	'CLASS_MEMBER': [
		['IDENTIFIER_SUBROUTINE'],
		['IDENTIFIER_VARIABLE'],
		['IDENTIFIER_PACKAGE']
	],
	'WHEN_EXPRESSION': [
		['KEYWORD_WHEN', 'GROUPER_LEFT_PARENTHESIS', 'OR_EXPRESSION', 'GROUPER_RIGHT_PARENTHESIS', 'OR_EXPRESSION', 'KEYWORD_ELSE', 'OR_EXPRESSION'],
	],
	'CAPTURE_LIST': [
		['UNARY_OPERATOR', 'DATA_NAMES', 'CAPTURE_LIST'],
		['DATA_NAMES', 'CAPTURE_LIST'],
		[]
	],
	'OPTIONAL_EXTRACTOR_EXPRESSION': [
		['SYMBOL_TILDE__PRUNE', 'DATA_NAMES', 'OPTIONAL_MEMBER_EXPRESSION'],
		[]
	],
	'OPTIONAL_CAPTURE_LIST': [
		['GROUPER_LEFT_BRACKET', 'CAPTURE_LIST', 'GROUPER_RIGHT_BRACKET'],
		[]
	],
	'ARRAY_ACCESS_EXPRESSION': [
		['GROUPER_LEFT_BRACKET', 'EXPRESSION_EXPRESSION', 'OPTIONAL_ARRAY_ACCESS_EXPRESSION', 'GROUPER_RIGHT_BRACKET', 'OPTIONAL_MEMBER_EXPRESSION']
	],
################################################################################ 27
	'OPTIONAL_ARRAY_ACCESS_EXPRESSION': [
		['SYMBOL_COMMA__PRUNE', 'EXPRESSION_EXPRESSION', 'OPTIONAL_ARRAY_ACCESS_EXPRESSION'],
		[]
	]
################################################################################
################################################################################
################################################################################
}


def createcodetblexerdependencyKeywordMatchercpp(terminal_set):
	template = '''%(license)s%(head)s\n\n%(namespace_head)s\n\n%(enumerations)s\n\n%(namespace_tail)s\n'''
	head = '''#include "KeywordMatcher.hpp"\n'''
	enumerations = '''%(function_signature)s\n{\n%(body)s\n}'''
	function_signature = '''protocols::TokenType KeywordMatcher::getKeyword(const std::string &lexeme)'''
	body = '''using namespace protocols;\nif (lexeme == "%(first)s") return TokenType::%(first_name)s;\n%(elifs)s\nelse return protocols::TokenType::UNIDENTIFIED;'''
	case = '''else if (lexeme == "%(symbol)s") return TokenType::%(symbol_name)s;'''

	namespace_head, namespace_tail = dependency.NamespaceGenerator.toNamespaces('tul', 'dependency')

	symbols = []
	for i in sorted(terminal_set):
		if i.startswith('KEYWORD_'):
			symbols.append([i[8:].lower(), i])
	symbols.sort(key=lambda x: len(x[0]), reverse=True)

	complete = [case % {'symbol': i[0], 'symbol_name': i[1]} for i in symbols[1:]]


	complete = body % {'elifs': '\n'.join(complete),'first': ''.join(symbols[0][0]), 'first_name': symbols[0][1]}
	complete = enumerations % {'body': dependency.Prepend.prependTabEachLine(complete),'function_signature': function_signature}
	complete = template % {'enumerations': complete, 'head': head, 'license': LICENSE_STRING, 'namespace_head': namespace_head, 'namespace_tail': namespace_tail}

	with open('./code/ast/tb/lexer/dependency/KeywordMatcher.cpp', 'w') as file:
		file.write(complete)


def createcodetblexerdependencySymbolMatchercpp(terminal_set):

	def convertSymbolSet(symbol_set):
		def convertSymbolNameToSymbol(name):
			if name == 'GREATER_THAN':
				return '>'
			elif name == 'LESS_THAN':
				return '<'
			elif name == 'SEMICOLON':
				return ';'
			elif name == 'COLON':
				return ':'
			elif name == 'COMMA':
				return ','
			elif name == 'DOT':
				return '.'
			elif name == 'APETAIL':
				return '@'
			elif name == 'DOLLAR':
				return '$'
			elif name == 'EXCLAMATION_MARK':
				return '!'
			elif name == 'CARET':
				return '^'
			elif name == 'BAR':
				return '|'
			elif name == 'AMPERSAND':
				return '&'
			elif name == 'BACKWARD_SLASH':
				return '\\\\'
			elif name == 'FORWARD_SLASH':
				return '/'
			elif name == 'STAR':
				return '*'
			elif name == 'MINUS':
				return '-'
			elif name == 'PLUS':
				return '+'
			elif name == 'EQUAL':
				return '='
			elif name == 'TILDE':
				return '~'
			elif name == 'PRUNE':
				return ''
			else:
				return "SYMBOL_NOT_FOUND_" + name
		set = []
		for i in symbol_set:
			set.append(convertSymbolNameToSymbol(i))
		return set

	template = '''%(license)s%(head)s\n\n%(namespace_head)s\n\n%(enumerations)s\n\n%(namespace_tail)s\n'''
	head = '''#include "SymbolMatcher.hpp"\n'''
	enumerations = '''%(function_signature)s\n{\n%(body)s\n}'''
	function_signature = '''protocols::TokenType SymbolMatcher::getSymbol(const std::string &lexeme)'''
	body = '''using namespace protocols;\nif (lexeme == "%(first)s") return TokenType::%(first_name)s;\n%(elifs)s\nelse return protocols::TokenType::UNIDENTIFIED;'''
	case = '''else if (lexeme == "%(symbol)s") return TokenType::%(symbol_name)s;'''

	namespace_head, namespace_tail = dependency.NamespaceGenerator.toNamespaces('tul', 'dependency')

	symbols = []
	for i in sorted(terminal_set):
		if i.startswith('SYMBOL_'):
			symbols.append([convertSymbolSet(i[7:].split('__')), i])
	symbols.sort(key=lambda x: len(x[0]), reverse=True)

	complete = [case % {'symbol': ''.join(i[0]), 'symbol_name': i[1]} for i in symbols[1:]]
	complete = body % {'elifs': '\n'.join(complete),'first': ''.join(symbols[0][0]), 'first_name': symbols[0][1]}
	complete = enumerations % {'body': dependency.Prepend.prependTabEachLine(complete),'function_signature': function_signature}
	complete = template % {'enumerations': complete, 'head': head, 'license': LICENSE_STRING, 'namespace_head': namespace_head, 'namespace_tail': namespace_tail}

	with open('./code/ast/tb/lexer/dependency/SymbolMatcher.cpp', 'w') as file:
		file.write(complete)


def createcodetbparserdependencyCrossTerminalParserinc():
	ParserTableGenerator.validateAllFirstSets(productions)
	lines_ = []
	for start_nonterminal in sorted(productions):
		transitions_ = ParserTableGenerator.computeTransitions(start_nonterminal, productions)
		lines_.append(ParserTableGenerator.generateTransitionMapCode(transitions_, productions))

	with open('./code/ast/tb/parser/dependency/CrossTerminalParser.inc', 'w') as file:
		file.write(LICENSE_STRING + '\n'.join(lines_))


def createcodetbparsertkcrTokenTypeToCrossTerminalcpp(terminal_set):
	template = '''%(license)s%(head)s\n%(namespace_head)s\n\n%(enumerations)s\n\n%(namespace_tail)s\n'''
	head = '''#include "TokenTypeToCrossTerminal.hpp"\n#include "protocols/CrossTerminal.hpp"\n#include "protocols/TokenType.hpp"\n\n'''
	enumerations = '''%(function_signature)s\n{\n%(body)s\n}'''
	function_signature = '''protocols::CrossTerminal TokenTypeToCrossTerminal::convertToCrossTerminal(protocols::TokenType token_type)'''
	body = '''switch (token_type)\n{\n%(cases)s\n}'''
	case = '''case protocols::TokenType::%(type)s: return protocols::CrossTerminal::%(type)s;\n'''

	namespace_head, namespace_tail = dependency.NamespaceGenerator.toNamespaces('tul', 'tkcr')

	complete = [case % {'type': i} for i in sorted(terminal_set)]
	complete.append('default: return protocols::CrossTerminal::UNIDENTIFIED;')
	complete = body % {'cases': dependency.Prepend.prependTabEachLine(''.join(complete))}
	complete = enumerations % {'body': dependency.Prepend.prependTabEachLine(complete),'function_signature': function_signature}
	complete = template % {'enumerations': complete,
	                       'head': head,
	                       'license': LICENSE_STRING,
	                       'namespace_head': namespace_head,
	                       'namespace_tail': namespace_tail}

	with open('./code/ast/tb/parser/tkcr/TokenTypeToCrossTerminal.cpp', 'w') as file:
		file.write(complete)


def createprotocolsCrossTerminalToolscpp(terminal_set, non_terminal_set):

	def generateIsExpression(non_terminal_set):
		is_keyword_function = '''bool CrossTerminalTools::isExpression(const CrossTerminal &ct_)\n{\n%(switches)s\n}'''
		switches = '''switch (ct_)\n{\n%(cases)s\n}'''
		cases = '''case CrossTerminal::%(ctname)s: return true;'''
		default = '''default: return false;'''

		result = [cases % {'ctname': i} for i in sorted(non_terminal_set) if i.endswith('_EXPRESSION')]
		result = switches % {'cases': dependency.Prepend.prependTabEachLine('\n'.join(result) + '\n' + default)}
		result = is_keyword_function % {'switches': dependency.Prepend.prependTabEachLine(result)}
		return result

	def generateIsKeyword(terminal_set):
		is_keyword_function = '''bool CrossTerminalTools::isKeyword(const CrossTerminal &ct_)\n{\n%(switches)s\n}'''
		switches = '''switch (ct_)\n{\n%(cases)s\n}'''
		cases = '''case CrossTerminal::%(ctname)s: return true;'''
		default = '''default: return false;'''

		result = [cases % {'ctname': i} for i in sorted(terminal_set) if i.startswith('KEYWORD_')]
		result = switches % {'cases': dependency.Prepend.prependTabEachLine('\n'.join(result) + '\n' + default)}
		result = is_keyword_function % {'switches': dependency.Prepend.prependTabEachLine(result)}
		return result

	def isUselessSymbol(terminal_set):
		is_keyword_function = '''bool CrossTerminalTools::isUselessSymbol(const CrossTerminal &ct_)\n{\n%(switches)s\n}'''
		switches = '''switch (ct_)\n{\n%(cases)s\n}'''
		cases = '''case CrossTerminal::%(ctname)s: return true;'''
		default = '''default: return false;'''

		result = [cases % {'ctname': i} for i in sorted(terminal_set) if i.startswith('SYMBOL_') and i.endswith('__PRUNE')]
		result = switches % {'cases': dependency.Prepend.prependTabEachLine('\n'.join(result) + '\n' + default)}
		result = is_keyword_function % {'switches': dependency.Prepend.prependTabEachLine(result)}
		return result

	def generateToString(terminal_set, non_terminal_set):
		enumerations = '''%(function_signature)s\n{\n%(body)s\n}'''
		function_signature = '''std::string CrossTerminalTools::toString(protocols::CrossTerminal cross_terminal)'''
		body = '''switch (cross_terminal)\n{\n%(cases)s\n}'''
		case = '''case protocols::CrossTerminal::%(type)s: return "%(type)s";\n'''

		complete = [case % {'type': i} for i in non_terminal_set]
		complete.extend([case % {'type': i} for i in terminal_set])
		complete.append('case protocols::CrossTerminal::ENUM_END: return "";')
		complete.sort()
		complete.append('default: return "";')
		complete = body % {'cases': dependency.Prepend.prependTabEachLine(''.join(complete))}
		complete = enumerations % {'body': dependency.Prepend.prependTabEachLine(complete),'function_signature': function_signature}
		return complete

	namespace_head, namespace_tail = dependency.NamespaceGenerator.toNamespaces('tul', 'protocols')
	template = '''%(license)s#include "CrossTerminalTools.hpp"\n\n\n%(namespace_head)s\n\n%(enumerations)s\n\n%(namespace_tail)s\n'''

	enumerations = [generateIsExpression(non_terminal_set)]
	enumerations.append(generateIsKeyword(terminal_set))
	enumerations.append(isUselessSymbol(terminal_set))
	enumerations.append(generateToString(terminal_set, non_terminal_set))
	result = template % {'enumerations': '\n\n'.join(enumerations),'license': LICENSE_STRING, 'namespace_head': namespace_head, 'namespace_tail': namespace_tail}

	with open('./protocols/CrossTerminalTools.cpp', 'w') as file:
		file.write(result)


def createprotocolsCrossTerminalhpp(terminal_set, non_terminal_set):
	template = '''%(license)s#pragma once\n\n%(namespace_head)s\n\nenum class CrossTerminal\n{\n%(enumerations)s\n};\n%(namespace_tail)s\n'''
	enumerations = ',\n'.join(sorted(non_terminal_set))
	enumerations += ',\n' + ',\n'.join(sorted(terminal_set))
	enumerations += ',\nENUM_END'

	namespace_head, namespace_tail = dependency.NamespaceGenerator.toNamespaces('tul', 'protocols')

	with open('./protocols/CrossTerminal.hpp', 'w') as file:
		file.write(template % {'enumerations': dependency.Prepend.prependTabEachLine(enumerations),
		                       'license': LICENSE_STRING,
		                       'namespace_head': namespace_head,
		                       'namespace_tail': namespace_tail})


def createprotocolsTokenTypehpp(terminal_set, non_terminal_set):
	template = '''%(license)s#pragma once\n\n%(namespace_head)s\n\nenum class TokenType\n{\n%(enumerations)s\n};\n\n%(namespace_tail)s\n'''
	enumerations = ',\n'.join(sorted(terminal_set))
	enumerations += ',\nENUM_END'

	namespace_head, namespace_tail = dependency.NamespaceGenerator.toNamespaces('tul', 'protocols')

	with open('./protocols/TokenType.hpp', 'w') as file:
		file.write(template % {'enumerations': dependency.Prepend.prependTabEachLine(enumerations),
		                       'license': LICENSE_STRING,
		                       'namespace_head': namespace_head,
		                       'namespace_tail': namespace_tail})


def enter():
	terminal_set, non_terminal_set = ParserTableGenerator.computeTerminals(productions)
	terminal_set |= {'UNIDENTIFIED', 'END_OF_MODULE'}
	non_terminal_set |= {'EPSILONATE'}

	createcodetblexerdependencyKeywordMatchercpp(terminal_set)
	createcodetblexerdependencySymbolMatchercpp(terminal_set)
	createcodetbparserdependencyCrossTerminalParserinc()
	createcodetbparsertkcrTokenTypeToCrossTerminalcpp(terminal_set)
	createprotocolsCrossTerminalToolscpp(terminal_set, non_terminal_set)
	createprotocolsCrossTerminalhpp(terminal_set, non_terminal_set)
	createprotocolsTokenTypehpp(terminal_set, non_terminal_set)


if __name__ == '__main__':
	enter()
