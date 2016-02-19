#! /usr/bin/env python3
import argparse

def enter():
	generator = util.Enum.generate
	enum = datru.EntryType.enum(generator)
	print(enum.name[0])
	lexer = lex.Lexer(enum)
	for i in 'hello woråld!':
		lexer.lex(i)
	lexer.cluster('a')
	#Parser.parse()
	#Semant.analyze()
	#Cogen.generate()


if __name__ == "__main__":
	enter()
