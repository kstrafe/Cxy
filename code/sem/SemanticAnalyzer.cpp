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
#include "SemanticAnalyzer.hpp"

#include <algorithm>
#include <iostream>

namespace tul { namespace sem {

template <typename F>
bool isAnyOf(protocols::CrossTerminal a, F f)
{
	return a == f;
}

template <typename F, typename ...Fs>
bool isAnyOf(protocols::CrossTerminal a, F f, Fs ...fs)
{
	return a == f || isAnyOf(a, fs...);
}

bool SemanticAnalyzer::checkTree(const protocols::SyntaxTree *tree) const
{
	using namespace protocols;
	bool correct = true;
	auto assertNoChildren = [&]() { return tree->children.size() == 0; };
	auto runs = [&]()
		{
			for (std::size_t i = 0; (i < tree->children.size()) && correct; ++i)
			{
				correct = checkTree(tree->children.at(i));
				if (correct == false)
					std::cout << tree->children.at(i)->toString() << std::endl;
			}
			return correct;
		};
	auto child = [&](std::size_t child_number)
		{ return tree->children.at(child_number)->node_type; };

	auto lexIsNum = [&]()
		{
			const std::string &lx = tree->token.accompanying_lexeme;
			return std::all_of(lx.begin(), lx.end(), ::isdigit);
		};
	auto lexIsData = [&]()
		{
			const std::string &lx = tree->token.accompanying_lexeme;
			return lx.find_first_not_of
				("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_")
				== std::string::npos;
		};

	auto anyExpression = [&](const CrossTerminal nodetype)
		{
			return isAnyOf(nodetype,
				CrossTerminal::OR_EXPRESSION,
				CrossTerminal::AND_EXPRESSION,
				CrossTerminal::BITWISE_OR_EXPRESSION,
				CrossTerminal::BITWISE_XOR_EXPRESSION,
				CrossTerminal::BITWISE_AND_EXPRESSION,
				CrossTerminal::EQUALITY_EXPRESSION,
				CrossTerminal::RELATIONAL_EXPRESSION,
				CrossTerminal::SHIFT_EXPRESSION,
				CrossTerminal::ADDITIVE_EXPRESSION,
				CrossTerminal::MULTIPLICATIVE_EXPRESSION,
				CrossTerminal::UNARY_EXPRESSION,

				CrossTerminal::CAST_EXPRESSION,
				CrossTerminal::MEMBER_EXPRESSION,
				CrossTerminal::SIZE_EXPRESSION,
				CrossTerminal::UNARY_EXPRESSION,

				CrossTerminal::MEMBER_EXPRESSION,
				CrossTerminal::INTEGER_LITERAL
				);
		};

	switch (tree->node_type)
	{
		// Internal (non-leaf) nodes
		case CrossTerminal::ENTER:
			correct &= isAnyOf(child(0), CrossTerminal::KEYWORD_VAR);
			return runs();
		case CrossTerminal::KEYWORD_VAR:
			correct &= isAnyOf(child(0), CrossTerminal::EPSILONATE,
				CrossTerminal::KEYWORD_PUBLIC, CrossTerminal::KEYWORD_PRIVATE,
				CrossTerminal::KEYWORD_RESTRICTED);
			correct &= isAnyOf(child(1), CrossTerminal::EPSILONATE,
				CrossTerminal::KEYWORD_GLOBAL);
			correct &= isAnyOf(child(2), CrossTerminal::TYPE);
			correct &= isAnyOf(child(3), CrossTerminal::DATA_NAMES);
			return runs();
		case CrossTerminal::TYPE:
			correct &= isAnyOf(child(0), CrossTerminal::EPSILONATE,
				CrossTerminal::AND_EXPRESSION);
			correct &= isAnyOf(child(1), CrossTerminal::PRIMITIVE_UNSIGNED,
				CrossTerminal::PRIMITIVE_SIGNED, CrossTerminal::PRIMITIVE_UNSIGNED_WRAPPED,
				CrossTerminal::PRIMITIVE_SIGNED_WRAPPED, CrossTerminal::TYPE);
			return runs();
		case CrossTerminal::DATA_NAMES:
			correct &= isAnyOf(child(0), CrossTerminal::EPSILONATE);
			correct |= anyExpression(child(0));
			correct &= isAnyOf(child(1), CrossTerminal::DATA_NAMES, CrossTerminal::EPSILONATE);
			correct &= lexIsData();
			return runs();

		case CrossTerminal::AND_EXPRESSION:
			correct &= anyExpression(child(0));
			correct &= anyExpression(child(1));
			return runs();
		case CrossTerminal::OR_EXPRESSION:
			correct &= anyExpression(child(0));
			correct &= anyExpression(child(1));
			return runs();
		case CrossTerminal::BITWISE_AND_EXPRESSION:
			correct &= anyExpression(child(0));
			correct &= anyExpression(child(1));
			return runs();
		case CrossTerminal::BITWISE_XOR_EXPRESSION:
			correct &= anyExpression(child(0));
			correct &= anyExpression(child(1));
			return runs();
		case CrossTerminal::BITWISE_OR_EXPRESSION:
			correct &= anyExpression(child(0));
			correct &= anyExpression(child(1));
			return runs();
		case CrossTerminal::EQUALITY_EXPRESSION:
			correct &= isAnyOf(child(0), CrossTerminal::SYMBOL_EQUAL__EQUAL, CrossTerminal::SYMBOL_EXCLAMATION_MARK__EQUAL);
			correct &= anyExpression(child(1));
			correct &= anyExpression(child(2));
			return runs();
		case CrossTerminal::MULTIPLICATIVE_EXPRESSION:
			correct &= isAnyOf(child(0), CrossTerminal::SYMBOL_STAR, CrossTerminal::SYMBOL_FORWARD_SLASH);
			correct &= anyExpression(child(1));
			correct &= anyExpression(child(2));
			return runs();

		case CrossTerminal::UNARY_EXPRESSION:
			correct &= isAnyOf(child(0), CrossTerminal::SYMBOL_APETAIL, CrossTerminal::SYMBOL_MINUS, CrossTerminal::SYMBOL_DOLLAR, CrossTerminal::SYMBOL_DOLLAR__DOLLAR, CrossTerminal::SYMBOL_EXCLAMATION_MARK, CrossTerminal::SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK);
			correct &= isAnyOf(child(1), CrossTerminal::CAST_EXPRESSION, CrossTerminal::DELETE_EXPRESSION, CrossTerminal::MEMBER_EXPRESSION, CrossTerminal::NEW_EXPRESSION, CrossTerminal::SIZE_EXPRESSION, CrossTerminal::UNARY_EXPRESSION, CrossTerminal::INTEGER_LITERAL);
			return runs();

		// Leaf nodes
		case CrossTerminal::INTEGER_LITERAL:
		case CrossTerminal::PRIMITIVE_SIGNED:
		case CrossTerminal::PRIMITIVE_UNSIGNED:
		case CrossTerminal::PRIMITIVE_SIGNED_WRAPPED:
		case CrossTerminal::PRIMITIVE_UNSIGNED_WRAPPED:
			correct &= lexIsNum();
			correct &= assertNoChildren();
			return correct;

		// These cases never have children.
		case CrossTerminal::EPSILONATE:
		case CrossTerminal::KEYWORD_DOUBLE:
		case CrossTerminal::KEYWORD_FLOAT:
		case CrossTerminal::KEYWORD_GLOBAL:
		case CrossTerminal::KEYWORD_PRIVATE:
		case CrossTerminal::KEYWORD_PUBLIC:
		case CrossTerminal::KEYWORD_RESTRICTED:
		case CrossTerminal::SYMBOL_APETAIL:
		case CrossTerminal::SYMBOL_DOLLAR:
		case CrossTerminal::SYMBOL_DOLLAR__DOLLAR:
		case CrossTerminal::SYMBOL_FORWARD_SLASH:
		case CrossTerminal::SYMBOL_MINUS:
		case CrossTerminal::SYMBOL_PLUS:
		case CrossTerminal::SYMBOL_STAR:
			return assertNoChildren();

		default: return false;
	}
}

}}
