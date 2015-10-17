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
#include "AbstractSyntaxTreeGenerator.hpp"
#include "tp/TreePruner.hpp"
#include "protocols/ConcreteSyntaxTree.hpp"

#include <iostream>


namespace tul { namespace ast {

	bool AbstractSyntaxTreeGenerator::buildTree(const std::string &code)
	{
		if (tree_builder.buildTree(code))
		{ return true; }
		return false;
	}

	bool AbstractSyntaxTreeGenerator::buildTree(char next_character)
	{
		return tree_builder.buildTree(next_character);
	}

	bool AbstractSyntaxTreeGenerator::endInput()
	{
		return tree_builder.endInput();
	}

	std::vector<std::string> AbstractSyntaxTreeGenerator::getExpectedTokens()
	{
		return std::move(tree_builder.getExpectedTokens());
	}

	bool AbstractSyntaxTreeGenerator::generateSemantics(const std::string &qualified_name)
	{
		std::unique_ptr<protocols::ConcreteSyntaxTree> tree = tree_builder.getConcreteSyntaxTree();
		tp::TreePruner::pruneTree(tree.get());
		table_builder.runOn(tree.get(), qualified_name);
		std::cout << tree.get()->toString() << std::endl;
		return true;
	}

}}
