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
import dependency.Grammar as Gram
import dependency.Prepend
import dependency.TableSorter as Tbs

# Take a list of cross-terminals and create the following files.
# Search for the word `file` in this code to find out where the folders get
# written to..
# Note: project root is the folder where readme.md is located (the root)

# These are the productions of the grammar.
# Any 'KEYWORD_' gets turned into an actual keyword recognizer (KeywordMatcher).
# The same occurs with 'SYMBOL_' (SymbolMatcher).
# Empty productions '[]' are epsilon productions.
# Generates a .inc file containing all lookahead rules.
productions = Gram.productions


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
			if name == 'PERCENT':
				return '%'
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

		result = [cases % {'ctname': i} for i in sorted(terminal_set) if i.startswith('SYMBOL_') and i.endswith('')]
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


def recomputeGrammar(terminal_set):
	p = dependency.Prepend.prependTabEachLine
	with open('./codegenerators/dependency/Grammar.py', 'w') as file:
		file.write(Tbs.sortTable(productions, terminal_set, p))


def enter():
	terminal_set, non_terminal_set = ParserTableGenerator.computeTerminals(productions)
	terminal_set |= {'UNIDENTIFIED', 'END_OF_MODULE'}
	non_terminal_set |= {'EPSILONATE'}

	recomputeGrammar(terminal_set)

	createcodetblexerdependencyKeywordMatchercpp(terminal_set)
	createcodetblexerdependencySymbolMatchercpp(terminal_set)
	createcodetbparserdependencyCrossTerminalParserinc()
	createcodetbparsertkcrTokenTypeToCrossTerminalcpp(terminal_set)
	createprotocolsCrossTerminalToolscpp(terminal_set, non_terminal_set)
	createprotocolsCrossTerminalhpp(terminal_set, non_terminal_set)
	createprotocolsTokenTypehpp(terminal_set, non_terminal_set)


if __name__ == '__main__':
	enter()
