--[[ tup.rule('$(ROOT)/codegenerators/CrossTerminal.py', 'python3 %f', {
	'$(ROOT)/code/ast/tb/lexer/dependency/KeywordMatcher.cpp',
	'$(ROOT)/code/ast/tb/parser/dependency/CrossTerminalParser.inc',
	'$(ROOT)/code/ast/tb/parser/tkcr/TokenTypeToCrossTerminal.cpp',
	'$(ROOT)/code/ast/tb/test_TreeBuilder.cpp',
	'$(ROOT)/protocols/CrossTerminal.hpp',
	'$(ROOT)/protocols/CrossTerminalTools.cpp',
	'$(ROOT)/protocols/TokenType.hpp'
})
--]]
tup.rule('$(ROOT)/temp/<objs>', 'g++ %<objs> -o %o', {'$(ROOT)/binaries/test.elf', '$(ROOT)/binaries/<test>'})
tup.rule({'$(ROOT)/binaries/<test>'}, './%<test>')
