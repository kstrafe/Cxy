# Complete Grammar #

This grammar is not finished yet. The grammar in CrossTerminal is the one that is currently used.

The complete EBNF grammar of the language is given here. As a notational standard:
	''         - enclosed characters are verbatim.
	{}         - means one or more
	[]         - means optional production
	UPPERCASE  - is a non-terminal
	lowercase  - is a terminal, a symbol coming from the lexer.

The grammar starts at ENTER. Most non-terminals are sorted alphabetically.

	ENTER ::=
		[ { ENTER } ]
		| TYPE { variablename [ '=' EXPRESSION ] } ';'
		| FUNCTION_SIGNATURE functionname '{' CODE_BLOCK '}'
		| 'grant' classname '{' GRANT_LIST '}'

	TYPE ::=
		ARRAY
		| BASIC_TYPE
		| 'const' TYPE_AFTER_CONST
		| 'ptr' TYPE_AFTER_PTR
		| 'ref' TYPE_AFTER_REF

	ARRAY ::= '[' EXPRESSION ',' TYPE ']'

	BASIC_TYPE ::=
		classname
		| packagename '.' classname
		| signed
		| unsigned
		| 'float'
		| 'double'

	TYPE_AFTER_CONST ::=
		ARRAY
		| BASIC_TYPE
		| 'ptr' TYPE

	TYPE_AFTER_PTR ::=
		ARRAY
		| BASIC_TYPE
		| FUNCTION_SIGNATURE
		| 'const' TYPE_AFTER_CONST
		| 'ptr' TYPE
