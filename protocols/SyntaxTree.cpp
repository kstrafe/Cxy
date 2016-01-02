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
#include "SyntaxTree.hpp"

#include <iomanip>
#include <sstream>


namespace tul { namespace protocols {

	void SyntaxTree::initToken()
	{
		token.entry_type = EntryType::WHITESPACE;
		token.line_number = 0;
		token.column_number = 0;
	}

	SyntaxTree::SyntaxTree()
	{
		initToken();
	}

	SyntaxTree::SyntaxTree(CrossTerminal ct)
	:
		node_type(ct)
	{
		initToken();
	}

	SyntaxTree::SyntaxTree(CrossTerminal ct, std::initializer_list<SyntaxTree *> subtrees)
	:
		node_type(ct),
		children(subtrees)
	{
		initToken();
	}

	SyntaxTree::~SyntaxTree()
	{
		for (SyntaxTree *child : children)
			delete child;
	}

	SyntaxTree *SyntaxTree::setLexeme(const std::string &lexeme)
	{
		token.accompanying_lexeme = lexeme;
		return this;
	}

	std::string SyntaxTree::toString(int indent) const
	{
		std::stringstream str_strm;
		str_strm << std::setfill('0') << std::setw(3) << indent << ':';
		std::string ind(indent, ' ');
		ind += str_strm.str();
		ind += protocols::CrossTerminalTools::toString(node_type);
		ind += '(';
		ind += token.accompanying_lexeme;
		ind += ") ";
		ind += std::to_string(token.line_number);
		ind += ':';
		ind += std::to_string(token.column_number);
		ind += '\n';
		for (auto child : children)
				ind += child->toString(indent + 2);
		return ind;
	}

}}
