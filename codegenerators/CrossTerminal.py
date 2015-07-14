# -*- coding: utf-8 -*-
# Copyright © 2015 Kevin Robert Stravers
"""
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
"""

LICENSE_STRING = '''// Copyright © 2015 Kevin Robert Stravers
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
'''

import ParserTableGenerator

# Take a list of cross-terminals and create the following files:
# Note: project is the folder where readme.md is located (the root)
# project/code/treebuilder/lexer/dependency/KeywordMatcher.cpp - Matches a lexeme with a TokenType, terminals starting with KEYWORD_ will be put there.
# project/code/treebuilder/lexer/dependency/SymbolMatcher.cpp - Matches a lexeme with a TokenType, terminals starting with SYMBOL_ will be put there.
# project/code/treebuilder/parser/dependency/CrossTerminalToString.cpp - A switch converter into std::string
# project/code/treebuilder/parser/dependency/TokenTypeToCrossTerminal.cpp - A switch that converts TokenType enum into CrossTerminal
# project/protocols/CrossTerminal.hpp - A simple enum containing all our CrossTerminals
# project/protocols/TokenType.hpp - A simple enum containing only terminals, without any non-terminals

terminals = {
    'GROUPER_LEFT_BRACE',
    'GROUPER_RIGHT_BRACE',
    'GROUPER_LEFT_BRACKET',
    'GROUPER_RIGHT_BRACKET',
    'GROUPER_LEFT_PARENTHESIS',
    'GROUPER_RIGHT_PARENTHESIS',

    'IDENTIFIER_CLASS',
    'IDENTIFIER_ENUMERATION',
    'IDENTIFIER_PACKAGE',
    'IDENTIFIER_SUBROUTINE',
    'IDENTIFIER_VARIABLE',

    'INTEGER_LITERAL',

    'KEYWORD_ASSEMBLY',
    'KEYWORD_DO',
    'KEYWORD_FOR',
    'KEYWORD_GOTO',
    'KEYWORD_IF',
    'KEYWORD_LABEL',
    'KEYWORD_PRIVATE',
    'KEYWORD_PUBLIC',
    'KEYWORD_RESTRICTED',
    'KEYWORD_WHILE',

    'PRIMITIVE_SIGNED',
    'PRIMITIVE_UNSIGNED',

    'STRING',

    'SYMBOL_PLUS',
    'SYMBOL_PLUS__PLUS',
    'SYMBOL_MINUS',
    'SYMBOL_MINUS__MINUS',
    'SYMBOL_STAR',
    'SYMBOL_STAR__STAR',
    'SYMBOL_FORWARD_SLASH',
    'SYMBOL_BACKWARD_SLASH',
    'SYMBOL_AMPERSAND__AMPERSAND',
    'SYMBOL_BAR__BAR',
    'SYMBOL_AMPERSAND',
    'SYMBOL_BAR',
    'SYMBOL_CARET',
    'SYMBOL_EXCLAMATION_MARK',
    'SYMBOL_EQUAL',
    'SYMBOL_EQUAL__EQUAL',
    'SYMBOL_DOT__DOT',
    'SYMBOL_DOT',
    'SYMBOL_COMMA',
    'SYMBOL_COLON',
    'SYMBOL_SEMICOLON',
    'SYMBOL_LESS_THAN__LESS_THAN',
    'SYMBOL_LESS_THAN',
    'SYMBOL_GREATER_THAN__GREATER_THAN',
    'SYMBOL_GREATER_THAN',

    'UNIDENTIFIED',
}


non_terminals = {
    'ENTER',
    'EXIT',
    'NONE',
    'FIELD_DECLARATION_LIST',
    'DECLARATION',
    'ACCESS_SPECIFIER',
    'OPTIONAL_EQUALS',
    'COMMA_SEPARATED_LIST',
    'OPTIONAL_CALL',
    'METHOD_DECLARATION_LIST',
    'COMMA_SEPARATED_TYPE_AND_NAME_LIST',
    'STATEMENT_LIST',
    'COMMA_SEPARATED_LIST_ITEM',
    'STATEMENT',
    'BASIC_TYPE',
    'OPTIONAL_ASSIGNMENT',
    'OPTIONAL_STRING_PLUS',
    'ARGUMENT_LIST',
    'FUNCTION_SIGNATURE',
    'SUBROUTINE_LIST',
    'SUBROUTINE',
    'EXPRESSION',
    'EPSILONATE'
}


productions = {
################################################################################
    'ENTER': [
        ['ACCESS_SPECIFICATION', 'DECL_OR_FUNC'],
        [],
    ],
################################################################################
    'ACCESS_SPECIFICATION': [
        ['ACCESS_SPECIFIER', 'OBJECT_ACCESS_SPECIFIER']
    ],
    'DECL_OR_FUNC': [
        ['FUNCTION_DEFINITION', 'FUNCTION_LIST'],
        ['DATA_DECLARATION', 'SYMBOL_SEMICOLON', 'ENTER'],
    ],
################################################################################
    'ACCESS_SPECIFIER': [
        ['KEYWORD_GLOBAL'],
        ['KEYWORD_GLOCAL'],
        []
    ],
    'OBJECT_ACCESS_SPECIFIER': [
        ['KEYWORD_PRIVATE'],
        ['KEYWORD_PUBLIC'],
        ['KEYWORD_RESTRICTED'],
    ],
    'FUNCTION_DEFINITION': [
        ['FUNCTION_SIGNATURE', 'IDENTIFIER_SUBROUTINE', 'GROUPER_LEFT_BRACE', 'STATEMENT_LIST', 'GROUPER_RIGHT_BRACE']
    ],
    'FUNCTION_LIST': [
        ['ACCESS_SPECIFICATION', 'FUNCTION_DEFINITION', 'FUNCTION_LIST'],
        []
    ],
    'DATA_DECLARATION': [
        ['TYPE', 'IDENTIFIER_VARIABLE', 'OPTIONAL_ASSIGNMENT', 'OPTIONAL_DATA_DECLARATION'],
    ],
################################################################################
    'FUNCTION_SIGNATURE': [
        ['GROUPER_LEFT_PARENTHESIS', 'ARGUMENT_LIST', 'SYMBOL_COLON', 'ARGUMENT_LIST', 'OPTIONAL_ATTRIBUTE_LIST', 'GROUPER_RIGHT_PARENTHESIS'],
    ],
    'STATEMENT_LIST': [
        ['STATEMENT', 'SYMBOL_SEMICOLON', 'STATEMENT_LIST'],
        ['NO_SEMICOLON_STATEMENT', 'STATEMENT_LIST'],
        []
    ],
    'OPTIONAL_ASSIGNMENT': [
        ['SYMBOL_EQUAL', 'EXPRESSION'],
        ['GROUPER_LEFT_PARENTHESIS', 'PARAMETER_LIST', 'GROUPER_RIGHT_PARENTHESIS'],
        []
    ],
    'TYPE': [
        ['BASIC_TYPE'],
        ['GROUPER_LEFT_BRACKET', 'INTEGER_LITERAL', 'SYMBOL_COMMA', 'TYPE', 'GROUPER_RIGHT_BRACKET'],
        ['KEYWORD_CONST', 'TYPE_AFTER_CONST'],
        ['KEYWORD_PTR', 'TYPE'],
        ['KEYWORD_REF', 'TYPE_AFTER_REF'],
    ],
    'OPTIONAL_DATA_DECLARATION': [
        ['SYMBOL_COMMA', 'IDENTIFIER_VARIABLE', 'OPTIONAL_ASSIGNMENT', 'OPTIONAL_DATA_DECLARATION'],
        [],
    ],
################################################################################
    'ARGUMENT_LIST': [
        ['ARGUMENT', 'OPTIONAL_ARGUMENT_LIST'],
        [],
    ],
    'OPTIONAL_ATTRIBUTE_LIST': [
        ['SYMBOL_COLON', 'ATTRIBUTE_LIST'],
        []
    ],
    'STATEMENT': [
        ['ASSIGNMENT'],
        ['DATA_DECLARATION'],
        ['CALL'],
        ['ITER_STATEMENT'],
        ['RETURN_STATEMENT']
    ],
    'NO_SEMICOLON_STATEMENT': [
        ['DO_STATEMENT'],
        ['FOR_STATEMENT'],
        ['GOTO_STATEMENT'],
        ['IF_STATEMENT'],
        ['LABEL_STATEMENT'],
        ['WHILE_STATEMENT'],
    ],
    'EXPRESSION': [
        ['ASSIGNMENT_EXPRESSION'],
    ],
    'TYPE_AFTER_REF': [
        ['BASIC_TYPE'],
        ['KEYWORD_PTR', 'TYPE'],
        ['KEYWORD_CONST', 'TYPE_AFTER_REF_CONST'],
    ],
    'BASIC_TYPE': [
        ['IDENTIFIER_PACKAGE', 'SYMBOL_DOT', 'IDENTIFIER_CLASS'],
        ['IDENTIFIER_CLASS'],
        ['PRIMITIVE_SIGNED'],
        ['PRIMITIVE_UNSIGNED'],
    ],
    'TYPE_AFTER_CONST': [
        ['BASIC_TYPE'],
        ['KEYWORD_PTR', 'TYPE'],
    ],
################################################################################
    'ARGUMENT': [
        ['TYPE', 'IDENTIFIER_VARIABLE', 'OPTIONAL_ASSIGNMENT'],
    ],
    'OPTIONAL_ARGUMENT_LIST': [
        ['SYMBOL_COMMA', 'ARGUMENT_LIST_AFTER_FIRST'],
        []
    ],
    'ATTRIBUTE_LIST': [
        ['KEYWORD_CONST', 'ATTRIBUTE_LIST_AFTER_CONST'],
        ['KEYWORD_PURE'],
        []
    ],
    'ASSIGNMENT': [
        ['IDENTIFIER_VARIABLE', 'OPTIONAL_MEMBER_EXPRESSION', 'SYMBOL_EQUAL', 'EXPRESSION'],
    ],
    'CALL': [
        ['IDENTIFIER_SUBROUTINE', 'GROUPER_LEFT_PARENTHESIS', 'PARAMETER_LIST', 'GROUPER_RIGHT_PARENTHESIS', 'OPTIONAL_EXTRACTOR']
    ],
    'ITER_STATEMENT': [
        ['SYMBOL_PLUS__PLUS', 'EXPRESSION'],
        ['SYMBOL_MINUS__MINUS', 'EXPRESSION'],
    ],
    'RETURN_STATEMENT': [
        ['KEYWORD_RETURN', 'PARAMETER_LIST'],
    ],
    'DO_STATEMENT': [
        ['KEYWORD_DO', 'GROUPER_LEFT_PARENTHESIS', 'EXPRESSION', 'GROUPER_RIGHT_PARENTHESIS', 'CODE_BLOCK']
    ],
    'FOR_STATEMENT': [
        ['KEYWORD_FOR', 'GROUPER_LEFT_PARENTHESIS', 'DATA_DECLARATION', 'SYMBOL_SEMICOLON', 'EXPRESSION', 'SYMBOL_SEMICOLON', 'STATEMENT_LIST', 'GROUPER_RIGHT_PARENTHESIS', 'CODE_BLOCK']
    ],
    'GOTO_STATEMENT': [
        ['KEYWORD_GOTO', 'IDENTIFIER_VARIABLE']
    ],
    'IF_STATEMENT': [
        ['KEYWORD_IF', 'GROUPER_LEFT_PARENTHESIS', 'EXPRESSION', 'GROUPER_RIGHT_PARENTHESIS', 'CODE_BLOCK']
    ],
    'LABEL_STATEMENT': [
        ['KEYWORD_LABEL', 'IDENTIFIER_VARIABLE']
    ],
    'WHILE_STATEMENT': [
        ['KEYWORD_WHILE', 'GROUPER_LEFT_PARENTHESIS', 'EXPRESSION', 'GROUPER_RIGHT_PARENTHESIS', 'CODE_BLOCK']
    ],
    'TYPE_AFTER_REF_CONST': [
        ['BASIC_TYPE'],
        ['KEYWORD_PTR', 'TYPE'],
        ['KEYWORD_CONST', 'TYPE_AFTER_REF_CONST'],
    ],
################################################################################
# EXPRESSION BLOCK.
# Contains all valid expressions in the language. These are sums, == compares...
# a + b * c - d
################################################################################
    'ASSIGNMENT_EXPRESSION': [
        ['OR_EXPRESSION', 'OPTIONAL_ASSIGNMENT_EXPRESSION']
    ],
################################################################################
    'OR_EXPRESSION': [
        ['AND_EXPRESSION', 'OPTIONAL_OR_EXPRESSION'],
    ],
    'OPTIONAL_ASSIGNMENT_EXPRESSION': [
        ['SYMBOL_EQUAL', 'OR_EXPRESSION'],
        ['SYMBOL_PLUS__EQUAL', 'OR_EXPRESSION'],
        ['SYMBOL_MINUS__EQUAL', 'OR_EXPRESSION'],
        ['SYMBOL_STAR__EQUAL', 'OR_EXPRESSION'],
        ['SYMBOL_FORWARD_SLASH__EQUAL', 'OR_EXPRESSION'],
        []
    ],
################################################################################
    'AND_EXPRESSION': [
        ['BITWISE_OR_EXPRESSION', 'OPTIONAL_AND_EXPRESSION'],
    ],
    'OPTIONAL_OR_EXPRESSION': [
        ['SYMBOL_BAR__BAR', 'OR_EXPRESSION'],
        []
    ],
################################################################################
    'BITWISE_OR_EXPRESSION': [
        ['BITWISE_XOR_EXPRESSION', 'OPTIONAL_BITWISE_OR_EXPRESSION'],
    ],
    'OPTIONAL_AND_EXPRESSION': [
        ['SYMBOL_AMPERSAND__AMPERSAND', 'AND_EXPRESSION'],
        []
    ],
################################################################################
    'BITWISE_XOR_EXPRESSION': [
        ['BITWISE_AND_EXPRESSION', 'OPTIONAL_BITWISE_XOR_EXPRESSION'],
    ],
    'OPTIONAL_BITWISE_OR_EXPRESSION': [
        ['SYMBOL_BAR', 'BITWISE_OR_EXPRESSION'],
        []
    ],
################################################################################
    'BITWISE_AND_EXPRESSION': [
        ['EQUALITY_EXPRESSION', 'OPTIONAL_BITWISE_AND_EXPRESSION'],
    ],
    'OPTIONAL_BITWISE_XOR_EXPRESSION': [
        ['SYMBOL_CARET', 'BITWISE_XOR_EXPRESSION'],
        []
    ],
################################################################################
    'EQUALITY_EXPRESSION': [
        ['RELATIONAL_EXPRESSION', 'OPTIONAL_EQUALITY_EXPRESSION'],
    ],
    'OPTIONAL_BITWISE_AND_EXPRESSION': [
        ['SYMBOL_AMPERSAND', 'BITWISE_AND_EXPRESSION'],
        []
    ],
################################################################################
    'RELATIONAL_EXPRESSION': [
        ['ADDITIVE_EXPRESSION', 'OPTIONAL_RELATIONAL_EXPRESSION'],
    ],
    'OPTIONAL_EQUALITY_EXPRESSION': [
        ['SYMBOL_EQUAL__EQUAL', 'EQUALITY_EXPRESSION'],
        []
    ],
################################################################################
    'ADDITIVE_EXPRESSION': [
        ['MULTIPLICATIVE_EXPRESSION', 'OPTIONAL_ADDITIVE_EXPRESSION'],
    ],
    'OPTIONAL_RELATIONAL_EXPRESSION': [
        ['RELATIONAL_OPERATOR', 'EQUALITY_EXPRESSION'],
        []
    ],
################################################################################
    'MULTIPLICATIVE_EXPRESSION': [
        ['CAST_EXPRESSION', 'OPTIONAL_MULTIPLICATIVE_EXPRESSION'],
    ],
    'OPTIONAL_ADDITIVE_EXPRESSION': [
        ['SYMBOL_PLUS', 'ADDITIVE_EXPRESSION'],
        ['SYMBOL_MINUS', 'ADDITIVE_EXPRESSION'],
        []
    ],
################################################################################
    'CAST_EXPRESSION': [
        ['UNARY_EXPRESSION'],
        ['KEYWORD_CAST', 'GROUPER_LEFT_PARENTHESIS', 'TYPE', 'GROUPER_RIGHT_PARENTHESIS', 'GROUPER_LEFT_BRACE', 'EXPRESSION', 'GROUPER_RIGHT_BRACE']
    ],
################################################################################
    'UNARY_EXPRESSION': [
        ['UNARY_OPERATOR', 'CAST_EXPRESSION'],
        ['MEMBER_EXPRESSION'],
    ],
    'OPTIONAL_MULTIPLICATIVE_EXPRESSION': [
        ['SYMBOL_STAR', 'MULTIPLICATIVE_EXPRESSION'],
        ['SYMBOL_FORWARD_SLASH', 'MULTIPLICATIVE_EXPRESSION'],
        []
    ],
################################################################################
    'MEMBER_EXPRESSION': [
        ['IDENTIFIER_CLASS', 'CLASS_MEMBER_EXPRESSION'],
        ['IDENTIFIER_PACKAGE', 'PACKAGE_MEMBER_EXPRESSION'],
        ['IDENTIFIER_SUBROUTINE', 'OPTIONAL_CALL'],
        ['RESOURCE', 'OPTIONAL_MEMBER_EXPRESSION'],
    ],

################################################################################
    'CLASS_MEMBER_EXPRESSION': [
        ['SYMBOL_DOT', 'CLASS_MEMBER', 'OPTIONAL_MEMBER_EXPRESSION'],
        ['GROUPER_LEFT_PARENTHESIS', 'PARAMETER_LIST', 'GROUPER_RIGHT_PARENTHESIS'],
    ],
    'PACKAGE_MEMBER_EXPRESSION': [
        ['SYMBOL_DOT', 'IDENTIFIER_CLASS', 'CLASS_MEMBER_EXPRESSION']
    ],
    'OPTIONAL_CALL': [
        ['GROUPER_LEFT_PARENTHESIS', 'PARAMETER_LIST', 'GROUPER_RIGHT_PARENTHESIS', 'SYMBOL_TILDE', 'IDENTIFIER_VARIABLE', 'OPTIONAL_MEMBER_EXPRESSION'],
        []
    ],
    'OPTIONAL_MEMBER_EXPRESSION': [
        ['SYMBOL_DOT', 'MEMBER_EXPRESSION'],
        ['GROUPER_LEFT_BRACKET', 'EXPRESSION', 'GROUPER_RIGHT_BRACKET', 'OPTIONAL_MEMBER_EXPRESSION'],
        ['GROUPER_LEFT_PARENTHESIS', 'PARAMETER_LIST', 'GROUPER_RIGHT_PARENTHESIS', 'SYMBOL_TILDE', 'IDENTIFIER_VARIABLE', 'OPTIONAL_MEMBER_EXPRESSION'],
        []
    ],
################################################################################
    'CLASS_MEMBER': [
        ['IDENTIFIER_SUBROUTINE'],
        ['IDENTIFIER_VARIABLE'],
    ],
################################################################################
################################################################################
################################################################################
    'UNARY_OPERATOR': [
        ['SYMBOL_APETAIL'],
        ['SYMBOL_APETAIL__APETAIL'],
        ['SYMBOL_DOLLAR'],
        ['SYMBOL_DOLLAR__DOLLAR'],
        ['SYMBOL_EXCLAMATION_MARK'],
        ['SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK'],
        ['SYMBOL_MINUS', 'CAST_EXPRESSION'],
    ],
    'RELATIONAL_OPERATOR': [
        ['SYMBOL_LESS_THAN'],
        ['SYMBOL_GREATER_THAN'],
        ['SYMBOL_LESS_THAN__EQUAL'],
        ['SYMBOL_GREATER_THAN__EQUAL'],
    ],
    'RESOURCE': [
        ['IDENTIFIER_VARIABLE'],
        ['STRING'],
        ['INTEGER_LITERAL'],
        ['GROUPER_LEFT_PARENTHESIS', 'EXPRESSION', 'GROUPER_RIGHT_PARENTHESIS']
    ],
    'ARGUMENT_LIST_AFTER_FIRST': [
        ['TYPE', 'IDENTIFIER_VARIABLE', 'OPTIONAL_ASSIGNMENT', 'OPTIONAL_ARGUMENT_LIST', 'OPTIONAL_ARGUMENT_LIST'],
        ['IDENTIFIER_VARIABLE', 'OPTIONAL_ARGUMENT_LIST', 'OPTIONAL_ARGUMENT_LIST'],
    ],
    'PARAMETER_LIST': [
        ['IDENTIFIER_VARIABLE', 'SYMBOL_COLON', 'EXPRESSION', 'OPTIONAL_PARAMETER_LIST'],
        []
    ],
    'OPTIONAL_EXTRACTOR': [
        ['SYMBOL_TILDE', 'IDENTIFIER_VARIABLE'],
        []
    ],
################################################################################
    'ATTRIBUTE_LIST_AFTER_CONST': [
        ['KEYWORD_PURE'],
        [],
    ],
    'CODE_BLOCK': [
        ['GROUPER_LEFT_BRACE', 'STATEMENT_LIST', 'GROUPER_RIGHT_BRACE']
    ],
    'OPTIONAL_PARAMETER_LIST': [
        ['SYMBOL_COMMA', 'PARAMETER_LIST'],
        []
    ]
}


def createcodetreebuilderlexerdependencyKeywordMatchercpp(terminal_set):
    header = LICENSE_STRING + '#include "KeywordMatcher.hpp"\n\n\nnamespace tul\n{\n\tnamespace treebuilder\n\t{\n\t\tnamespace lexer\n\t\t{\n\t\t\tnamespace dependency\n\t\t\t{\n\t\t\t\tprotocols::TokenType KeywordMatcher::getKeyword(const std::string &lexeme)\n\t\t\t\t{\n\t\t\t\t\t'
    footer = '\t\t\t\t\telse return protocols::TokenType::UNIDENTIFIED;\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\t}\n}'
    with open('./code/treebuilder/lexer/dependency/KeywordMatcher.cpp', 'w') as file:
        file.write(header)
        first_keyword = True
        for i in sorted(terminal_set):
            if i.startswith('KEYWORD_'):
                if first_keyword:
                    first_keyword = False
                    file.write('if (lexeme == "' + i.split('_')[1].lower() + '") return protocols::TokenType::' + i + ';\n')
                else:
                    file.write('\t\t\t\t\telse if (lexeme == "' + i.split('_')[1].lower() + '") return protocols::TokenType::' + i + ';\n')
        file.write(footer)


def createcodetreebuilderlexerdependencySymbolMatchercpp(terminal_set):

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
            else:
                return "SYMBOL_NOT_FOUND_" + name
        set = []
        for i in symbol_set:
            set.append(convertSymbolNameToSymbol(i))
        return set

    header = LICENSE_STRING + '#include "SymbolMatcher.hpp"\n\n\nnamespace tul\n{\n\tnamespace treebuilder\n\t{\n\t\tnamespace lexer\n\t\t{\n\t\t\tnamespace dependency\n\t\t\t{\n\t\t\t\tprotocols::TokenType SymbolMatcher::getSymbol(const std::string &lexeme)\n\t\t\t\t{\n\t\t\t\t\tusing namespace protocols;\n\t\t\t\t\t'
    footer = '\t\t\t\t\telse return protocols::TokenType::UNIDENTIFIED;\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\t}\n}'
    symbols = []
    for i in sorted(terminal_set):
        if i.startswith('SYMBOL_'):
            symbols.append([convertSymbolSet(i[7:].split('__')), i])

    symbols.sort(key=lambda x: len(x[0]), reverse=True)

    with open('./code/treebuilder/lexer/dependency/SymbolMatcher.cpp', 'w') as file:
        file.write(header)
        first_keyword = True
        for i in sorted(symbols, reverse=True):
            if first_keyword:
                first_keyword = False
                file.write('if (lexeme == "' + ''.join(i[0]) + '") return protocols::TokenType::' + i[1] + ';\n')
            else:
                file.write('\t\t\t\t\telse if (lexeme == "' + ''.join(i[0]) + '") return protocols::TokenType::' + i[1] + ';\n')
        file.write(footer)


def createcodetreebuilderparserdependencyCrossTerminalParserinc():
    ParserTableGenerator.validateAllFirstSets(productions)
    lines_ = []
    for start_nonterminal in sorted(productions):
        transitions_ = ParserTableGenerator.computeTransitions(start_nonterminal, productions)
        lines_.append(ParserTableGenerator.generateTransitionMapCode(transitions_, productions))

    with open('./code/treebuilder/parser/dependency/CrossTerminalParser.inc', 'w') as file:
        file.write(LICENSE_STRING + '\n'.join(lines_))


def createcodetreebuilderparserdependencyCrossTerminalToStringcpp(terminal_set, non_terminal_set):
    header = LICENSE_STRING + '#include "CrossTerminalToString.hpp"\n\n\nnamespace tul\n{\n\tnamespace treebuilder\n\t{\n\t\tnamespace parser\n\t\t{\n\t\t\tnamespace dependency\n\t\t\t{\n\t\t\t\tstd::string CrossTerminalToString::convertToString(protocols::CrossTerminal cross_terminal)\n\t\t\t\t{\n\t\t\t\t\tswitch (cross_terminal)\n\t\t\t\t\t{\n'
    footer = '\t\t\t\t\t\tdefault: return "";\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\t}\n}'
    with open('./code/treebuilder/parser/dependency/CrossTerminalToString.cpp', 'w') as file:
        file.write(header)
        for i in sorted(non_terminal_set):
            file.write('\t\t\t\t\t\tcase protocols::CrossTerminal::' + i + ': return "' + i + '";\n')
        for i in sorted(terminal_set):
            file.write('\t\t\t\t\t\tcase protocols::CrossTerminal::' + i + ': return "' + i + '";\n')
        file.write(footer)


def createcodetreebuilderparserdependencyTokenTypeToCrossTerminalcpp(terminal_set):
    header = LICENSE_STRING + '#include "TokenTypeToCrossTerminal.hpp"\n\n\nnamespace tul\n{\n\tnamespace treebuilder\n\t{\n\t\tnamespace parser\n\t\t{\n\t\t\tnamespace dependency\n\t\t\t{\n\t\t\t\tprotocols::CrossTerminal TokenTypeToCrossTerminal::convertToCrossTerminal(protocols::TokenType token_type)\n\t\t\t\t{\n\t\t\t\t\tswitch (token_type)\n\t\t\t\t\t{\n'
    footer = '\t\t\t\t\t\tdefault: return protocols::CrossTerminal::UNIDENTIFIED;\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\t}\n}'
    with open('./code/treebuilder/parser/dependency/TokenTypeToCrossTerminal.cpp', 'w') as file:
        file.write(header)
        for i in sorted(terminal_set):
            file.write('\t\t\t\t\t\tcase protocols::TokenType::' + i + ': return protocols::CrossTerminal::' + i + ';\n')
        file.write(footer)


def createprotocolsCrossTerminalhpp(terminal_set, non_terminals):
    header = LICENSE_STRING + '#pragma once\n\nnamespace tul\n{\n\tnamespace protocols\n\t{\n\t\tenum class CrossTerminal\n\t\t{\n'
    footer = '\t\t\tENUM_END\n\t\t};\n\t}\n}'
    with open('./protocols/CrossTerminal.hpp', 'w') as file:
        file.write(header)
        for i in sorted(non_terminals):
            file.write('\t\t\t' + i + ',\n')
        for i in sorted(terminal_set):
            file.write('\t\t\t' + i + ',\n')
        file.write(footer)


def createprotocolsTokenTypehpp(terminal_set, non_terminal_set):
    header = LICENSE_STRING + '#pragma once\n\nnamespace tul\n{\n\tnamespace protocols\n\t{\n\t\tenum class TokenType\n\t\t{\n'
    footer = '\t\t\tENUM_END\n\t\t};\n\t}\n}'
    with open('./protocols/TokenType.hpp', 'w') as file:
        file.write(header)
        for i in sorted(terminal_set):
            file.write('\t\t\t' + i + ',\n')
        file.write(footer)


def enterMain():
    terminal_set, non_terminal_set = ParserTableGenerator.computeTerminals(productions)
    terminal_set |= terminals
    non_terminal_set |= non_terminals

    createcodetreebuilderlexerdependencyKeywordMatchercpp(terminal_set)
    createcodetreebuilderlexerdependencySymbolMatchercpp(terminal_set)
    createcodetreebuilderparserdependencyCrossTerminalParserinc()
    createcodetreebuilderparserdependencyCrossTerminalToStringcpp(terminal_set, non_terminal_set)
    createcodetreebuilderparserdependencyTokenTypeToCrossTerminalcpp(terminal_set)
    createprotocolsCrossTerminalhpp(terminal_set, non_terminal_set)
    createprotocolsTokenTypehpp(terminal_set, non_terminal_set)


if __name__ == '__main__':
    enterMain()
