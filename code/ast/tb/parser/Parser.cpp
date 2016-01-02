// Copyright © 2015-2016 Kevin Robert Stravers
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
#include "Parser.hpp"
#include "protocols/CrossTerminalTools.hpp"
#include "tkcr/TokenTypeToCrossTerminal.hpp"


namespace tul { namespace parser {

Parser::Parser()
{
	using namespace protocols;
	symbol_stack.emplace(&end_of_input_tree);

	syntax_tree_root.reset(new SyntaxTree);
	symbol_stack.emplace(syntax_tree_root.get());
	syntax_tree_root->node_type = CrossTerminal::ENTER;
}

protocols::SyntaxTree Parser::end_of_input_tree = protocols::SyntaxTree(protocols::CrossTerminal::END_OF_MODULE);

Parser::~Parser()
{
}


bool Parser::isEmpty()
{
	return symbol_stack.empty();
}


std::unique_ptr<protocols::SyntaxTree> Parser::getSyntaxTree()
{
	return std::move(syntax_tree_root);
}


/**
	Here we simply decode the token's symbol into a CrossTerminal
	and feed it to the CrossTerminalParser. This will be a big switch for
	converting the TokenType into a CrossTerminal, in addition to keeping
	a stack containing all bottom nodes of the production tree.

	The CrossTerminalParser will return a vector of new children and an
	action. The CrossTerminalParser's job is to set up the parsing
	table.
*/
bool Parser::parseSymbol(const protocols::Token &input_token)
{
	using namespace protocols;
	CrossTerminal cross_terminal = tkcr::TokenTypeToCrossTerminal::convertToCrossTerminal(input_token.token_type);
	while (symbol_stack.empty() == false)
	{
		ParseReturn<protocols::CrossTerminal> parse_return = cross_parser.parseSymbol(symbol_stack.top()->node_type, cross_terminal);
		switch (parse_return.desired_action)
		{
			case ParseReturn<CrossTerminal>::Action::REMOVE_TOP: // Match, also remove from the input stack, by returning, we imply this.
				symbol_stack.top()->token = input_token;
				symbol_stack.top()->node_type = cross_terminal;
				symbol_stack.pop();
				return true;
			case ParseReturn<CrossTerminal>::Action::EPSILONATE:
				symbol_stack.top()->node_type = CrossTerminal::EPSILONATE;
				symbol_stack.pop();
				continue;
			case ParseReturn<CrossTerminal>::Action::CONTINUE:
			{
				SyntaxTree *tree_ptr = symbol_stack.top();
				symbol_stack.pop();
				for (std::size_t i = parse_return.child_symbols->size() - 1; i >= 0; --i)
				{
					SyntaxTree *child = new SyntaxTree;
					child->node_type = (*parse_return.child_symbols)[i];

					symbol_stack.push(child);
					tree_ptr->children.push_front(child);
					if (i == 0)
						break;
				}
				continue;
			}
			break;
			case ParseReturn<CrossTerminal>::Action::OBSERVE_ERROR:
				return false;
			default:
				return false;
		}
	}

	return false;
}


std::vector<std::string> Parser::formulateExpectedTokens() const
{
	using namespace protocols;
	if (symbol_stack.empty())
		return {};
	CrossTerminal cross_t = symbol_stack.top()->node_type;
	std::vector<CrossTerminal> expected_tokens = cross_parser.calculateExpectedTokens(cross_t);

	expected_tokens.insert(expected_tokens.begin(), cross_t);

	std::vector<std::string> serialized_tokens;
	for (CrossTerminal ct : expected_tokens)
	{
		serialized_tokens.emplace_back(protocols::CrossTerminalTools::toString(ct));
	}
	return serialized_tokens;
}

}}
