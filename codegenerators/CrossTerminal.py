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

# Take a list of cross-terminals and create the following files:
# Note: project is the folder where readme.md is located (the root)
# project/code/treebuilder/lexer/dependency/KeywordMatcher.cpp - Matches a lexeme with a TokenType, terminals starting with KEYWORD_ will be put there.
# project/code/treebuilder/lexer/dependency/SymbolMatcher.cpp - Matches a lexeme with a TokenType, terminals starting with SYMBOL_ will be put there.
# project/code/treebuilder/parser/dependency/CrossTerminalToString.cpp - A switch converter into std::string
# project/code/treebuilder/parser/dependency/TokenTypeToCrossTerminal.cpp - A switch that converts TokenType enum into CrossTerminal
# project/protocols/CrossTerminal.hpp - A simple enum containing all our CrossTerminals
# project/protocols/TokenType.hpp - A simple enum containing only terminals, without any non-terminals

terminals = [
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
    'SYMBOL_COLON__COLON',
    'SYMBOL_COLON',
    'SYMBOL_SEMICOLON',
    'SYMBOL_LESS_THAN__LESS_THAN',
    'SYMBOL_LESS_THAN',
    'SYMBOL_GREATER_THAN__GREATER_THAN',
    'SYMBOL_GREATER_THAN',

    'UNIDENTIFIED',
]


non_terminals = [
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
    'TYPE',
    'OPTIONAL_ASSIGNMENT',
    'OPTIONAL_STRING_PLUS',
    'ARGUMENT_LIST',
    'FUNCTION_SIGNATURE',
    'SUBROUTINE_LIST',
    'SUBROUTINE',
    'EXPRESSION',
    'EPSILONATE'
]


def createcodelexerdependencyKeywordMatchercpp():
    header = LICENSE_STRING + '#include "KeywordMatcher.hpp"\n\n\nnamespace tul\n{\n\tnamespace lexer\n\t{\n\t\tnamespace dependency\n\t\t{\n\t\t\tprotocols::TokenType KeywordMatcher::getKeyword(const std::string &lexeme)\n\t\t\t{\n\t\t\t\t'
    footer = '\t\t\t\telse return protocols::TokenType::UNIDENTIFIED;\n\t\t\t}\n\t\t}\n\t}\n}'
    with open('./code/treebuilder/lexer/dependency/KeywordMatcher.cpp', 'w') as file:
        file.write(header)
        first_keyword = True
        for i in terminals:
            if i.startswith('KEYWORD_'):
                if first_keyword:
                    first_keyword = False
                    file.write('if (lexeme == "' + i.split('_')[1].lower() + '") return protocols::TokenType::' + i + ';\n')
                else:
                    file.write('\t\t\t\telse if (lexeme == "' + i.split('_')[1].lower() + '") return protocols::TokenType::' + i + ';\n')
        file.write(footer)


def createcodelexerdependencyKeywordMatchercpp():

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
            else:
                return "SYMBOL_NOT_FOUND_" + name
        set = []
        for i in symbol_set:
            set.append(convertSymbolNameToSymbol(i))
        return set

    header = LICENSE_STRING + '#include "SymbolMatcher.hpp"\n\n\nnamespace tul\n{\n\tnamespace treebuilder\n\t{\n\t\tnamespace lexer\n\t\t{\n\t\t\tnamespace dependency\n\t\t\t{\n\t\t\t\tprotocols::TokenType SymbolMatcher::getSymbol(const std::string &lexeme)\n\t\t\t\t{\n\t\t\t\t\tusing namespace protocols;\n\t\t\t\t\t'
    footer = '\t\t\t\t\telse return protocols::TokenType::UNIDENTIFIED;\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\t}\n}'
    symbols = []
    for i in terminals:
        if i.startswith('SYMBOL_'):
            symbols.append([convertSymbolSet(i[7:].split('__')), i])

    symbols.sort(key=lambda x: len(x[0]), reverse=True)

    with open('./code/treebuilder/lexer/dependency/SymbolMatcher.cpp', 'w') as file:
        file.write(header)
        first_keyword = True
        for i in symbols:
            if first_keyword:
                first_keyword = False
                file.write('if (lexeme == "' + ''.join(i[0]) + '") return protocols::TokenType::' + i[1] + ';\n')
            else:
                file.write('\t\t\t\t\telse if (lexeme == "' + ''.join(i[0]) + '") return protocols::TokenType::' + i[1] + ';\n')
        file.write(footer)


def createcodeparserdependencyCrossTerminalToStringcpp():
    header = LICENSE_STRING + '#include "CrossTerminalToString.hpp"\n\n\nnamespace tul\n{\n\tnamespace treebuilder\n\t{\n\t\tnamespace parser\n\t\t{\n\t\t\tnamespace dependency\n\t\t\t{\n\t\t\t\tstd::string CrossTerminalToString::convertToString(protocols::CrossTerminal cross_terminal)\n\t\t\t\t{\n\t\t\t\t\tswitch (cross_terminal)\n\t\t\t\t\t{\n'
    footer = '\t\t\t\t\t\tdefault: return "";\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\t}\n}'
    with open('./code/treebuilder/parser/dependency/CrossTerminalToString.cpp', 'w') as file:
        file.write(header)
        for i in non_terminals:
            file.write('\t\t\t\t\t\tcase protocols::CrossTerminal::' + i + ': return "' + i + '";\n')
        for i in terminals:
            file.write('\t\t\t\t\t\tcase protocols::CrossTerminal::' + i + ': return "' + i + '";\n')
        file.write(footer)


def createcodeparserdependencyTokenTypeToCrossTerminalcpp():
    header = LICENSE_STRING + '#include "TokenTypeToCrossTerminal.hpp"\n\n\nnamespace tul\n{\n\tnamespace treebuilder\n\t{\n\t\tnamespace parser\n\t\t{\n\t\t\tnamespace dependency\n\t\t\t{\n\t\t\t\tprotocols::CrossTerminal TokenTypeToCrossTerminal::convertToCrossTerminal(protocols::TokenType token_type)\n\t\t\t\t{\n\t\t\t\t\tswitch (token_type)\n\t\t\t\t\t{\n'
    footer = '\t\t\t\t\t\tdefault: return protocols::CrossTerminal::UNIDENTIFIED;\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\t}\n}'
    with open('./code/treebuilder/parser/dependency/TokenTypeToCrossTerminal.cpp', 'w') as file:
        file.write(header)
        for i in terminals:
            file.write('\t\t\t\t\t\tcase protocols::TokenType::' + i + ': return protocols::CrossTerminal::' + i + ';\n')
        file.write(footer)


def createprotocolsCrossTerminalhpp():
    header = LICENSE_STRING + '#pragma once\n\nnamespace tul\n{\n\tnamespace protocols\n\t{\n\t\tenum class CrossTerminal\n\t\t{\n'
    footer = '\t\t\tENUM_END\n\t\t};\n\t}\n}'
    with open('./protocols/CrossTerminal.hpp', 'w') as file:
        file.write(header)
        for i in non_terminals:
            file.write('\t\t\t' + i + ',\n')
        for i in terminals:
            file.write('\t\t\t' + i + ',\n')
        file.write(footer)


def createprotocolsTokenTypehpp():
    header = LICENSE_STRING + '#pragma once\n\nnamespace tul\n{\n\tnamespace protocols\n\t{\n\t\tenum class TokenType\n\t\t{\n'
    footer = '\t\t\tENUM_END\n\t\t};\n\t}\n}'
    with open('./protocols/TokenType.hpp', 'w') as file:
        file.write(header)
        for i in terminals:
            file.write('\t\t\t' + i + ',\n')
        file.write(footer)


def enterMain():
    createcodelexerdependencyKeywordMatchercpp()
    createcodeparserdependencyCrossTerminalToStringcpp()
    createcodeparserdependencyTokenTypeToCrossTerminalcpp()
    createprotocolsCrossTerminalhpp()
    createprotocolsTokenTypehpp()


if __name__ == '__main__':
    enterMain()
