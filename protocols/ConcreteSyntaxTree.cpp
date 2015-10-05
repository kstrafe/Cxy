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
#include "ConcreteSyntaxTree.hpp"

#include <iomanip>
#include <sstream>


namespace tul { namespace protocols {

		ConcreteSyntaxTree::ConcreteSyntaxTree()
		{
			token_.entry_type = EntryType::WHITESPACE;
		}

		ConcreteSyntaxTree::ConcreteSyntaxTree(CrossTerminal ct_)
		:
			node_type(ct_)
		{}

		ConcreteSyntaxTree::~ConcreteSyntaxTree()
		{
			for (ConcreteSyntaxTree *child_ : children_)
				delete child_;
		}

		std::string ConcreteSyntaxTree::toString(int indent)
		{
			std::stringstream str_strm;
			str_strm << std::setfill('0') << std::setw(3) << indent << ':';
			std::string ind(indent, ' ');
			ind += str_strm.str();
			ind += protocols::CrossTerminalTools::toString(node_type);
			ind += '(';
			ind += token_.accompanying_lexeme;
			ind += ')';
			ind += '\n';
			for (auto child_ : children_)
					ind += child_->toString(indent + 2);
			return ind;
		}

}}
