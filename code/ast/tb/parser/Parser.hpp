// Copyright © 2015 Kevin Robert Stravers
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
#pragma once
#include "dependency/CrossTerminalParser.hpp"
#include "protocols/SyntaxTree.hpp"
#include "protocols/CrossTerminal.hpp"
#include "protocols/Token.hpp"

#include <memory>
#include <stack>
#include <string>


namespace tul { namespace parser {

/**
	Implement a simple LL(1) parser for our language. This is a mathematical model of
	a parser. It runs in O(n) time. The protocols used will be the Token and SyntaxTree.

	The parser will be table/state based as it has to be re-entrant. The mealy machine can be of
	use here.

	The parser simply looks up the current lookahead and the top token on the stack. We can
	idealize the top token on the stack as the state.

	Entry [state][lookahead] will give us a result from the mealy machine. For example, such a
	result will be the action "transfer with '(', 'boolean_expression', ')'". This will put those
	tokens on top of the stack. It will also need to link the last token into a tree somehow.

	transfer[top][input] => {New, Terminals, And, Non-Terminals}
*/
class Parser
{
public:

	Parser();
	~Parser();
	bool isEmpty();
	bool parseSymbol(const protocols::Token &input_token);
	std::unique_ptr<protocols::SyntaxTree> getSyntaxTree();

	std::vector<std::string> formulateExpectedTokens() const;

private:

	static protocols::SyntaxTree end_of_input_tree;
	dependency::CrossTerminalParser cross_parser;
	std::unique_ptr<protocols::SyntaxTree> syntax_tree_root;
	std::stack<protocols::SyntaxTree *> symbol_stack;

};

}}
