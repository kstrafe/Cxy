#! /usr/bin/env python3
import argparse

def enter():
	generator = util.Enum.generate
	entrytypes = datru.EntryType.enum(generator)
	lexer = lex.Lexer(entrytypes)
	for i in '(Hello World!)\tSomething = `good```+1/3':
		lexer.lex(i)
	#Parser.parse()
	#Semant.analyze()
	#Cogen.generate()


if __name__ == "__main__":
	enter()
