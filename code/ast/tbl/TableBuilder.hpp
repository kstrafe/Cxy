// Copyright © 2015 Kevin Robert Stravers
/*
This file is part of Unnamed-Language Compiler Reference Implementation (ULCRI).

ULCRI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ULCRI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ULCRI.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "sym/SymbolTable.hpp"
#include "protocols/ConcreteSyntaxTree.hpp"


namespace tul { namespace tbl {

	class TableBuilder
	{
	public:

		TableBuilder();
		~TableBuilder();

		bool runOn(protocols::ConcreteSyntaxTree *ct_root);

	private:

		bool collectFunctionInformation(protocols::ConcreteSyntaxTree *ct_root);
		bool collectFunctionInformationAfterEnter(protocols::ConcreteSyntaxTree *ct_root);
		bool collectFunctionSignature(protocols::ConcreteSyntaxTree *ct_root, sym::MethodTable &mod_tab);
		bool collectScopeInformation(protocols::ConcreteSyntaxTree *ct_root);

		sym::SymbolTable module_symtab;
	};

}}