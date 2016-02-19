#! /usr/bin/env python3
import argparse

def enter():
	lexer = lex.Lexer()
	for i in 'hello woråld!':
		lexer.lex(i)
	lexer.cluster('a')
	print(dir(lex))
	print(dir(lex.Lexer))
	#Parser.parse()
	#Semant.analyze()
	#Cogen.generate()

if __name__ == "__main__":
	enter()
