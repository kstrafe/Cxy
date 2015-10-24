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
	auto child = [&](std::size_t child_number){ return tree->children.at(child_number)->node_type; };
	switch (tree->node_type)
	{
		case CrossTerminal::ENTER: isAnyOf(child(0), CrossTerminal::KEYWORD_VAR);
		default: return false;
	}
}

}}
