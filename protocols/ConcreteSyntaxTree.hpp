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
#include "CrossTerminal.hpp"
#include "CrossTerminalTools.hpp"
#include "Token.hpp"

#include <deque>
#include <initializer_list>
#include <string>


namespace tul { namespace protocols {

		/**
			The concrete syntax tree is a data structure generated by the parser.
			It contains the type of node, token information, and productions (
			children).
		*/
		struct ConcreteSyntaxTree
		{
			void initToken();
			ConcreteSyntaxTree();
			ConcreteSyntaxTree(CrossTerminal ct);
			ConcreteSyntaxTree(CrossTerminal ct, std::initializer_list<ConcreteSyntaxTree *> subtrees);
			~ConcreteSyntaxTree();
			std::string toString(int indent = 0);

			ConcreteSyntaxTree *setLexeme(const std::string &lexeme);
			Token token;
			CrossTerminal node_type;
			std::deque<ConcreteSyntaxTree *> children;
		};

}}
