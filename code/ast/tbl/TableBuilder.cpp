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
#include "protocols/CrossTerminalTools.hpp"
#include "TableBuilder.hpp"

#include <cassert>
#include <iostream>


namespace tul { namespace tbl {

namespace {

protocols::CrossTerminal getAccessSpecifier
(
	const protocols::ConcreteSyntaxTree *ct_root
)
{
	using namespace protocols;
	switch (ct_root->node_type)
	{
		case CrossTerminal::ENTER:
			return getAccessSpecifier(ct_root->children_.at(0));
		case CrossTerminal::ACCESS_SPECIFICATION:
			return getAccessSpecifier(ct_root->children_.at(0));
		case CrossTerminal::ACCESS_SPECIFIER:
			return ct_root->children_.at(0)->node_type;
		default:
			return CrossTerminal::EPSILONATE;
	}
}

protocols::CrossTerminal getObjectSpecifier
(
	const protocols::ConcreteSyntaxTree *ct_root
)
{
	using namespace protocols;
	switch (ct_root->node_type)
	{
		case CrossTerminal::ENTER:
			return getObjectSpecifier(ct_root->children_.at(0));
		case CrossTerminal::ACCESS_SPECIFICATION:
			return getObjectSpecifier(ct_root->children_.at(1));
		case CrossTerminal::OBJECT_ACCESS_SPECIFIER:
			return ct_root->children_.at(0)->node_type;
		default:
			return CrossTerminal::EPSILONATE;
	}
}

void assignSignature
(
	const protocols::ConcreteSyntaxTree *ct_root,
	sym::Entry &entry_
)
{
	using namespace protocols;
	switch (ct_root->node_type)
	{
		case CrossTerminal::ENTER:
			assignSignature(ct_root->children_.at(1), entry_);
			break;

		case CrossTerminal::DECL_OR_FUNC:
			assignSignature(ct_root->children_.at(0), entry_);
			break;

		case CrossTerminal::DATA_DECLARATION:
			assignSignature(ct_root->children_.at(0), entry_);
			assignSignature(ct_root->children_.at(1), entry_);
			break;

		case CrossTerminal::IDENTIFIER_VARIABLE:
			entry_.method_identifier = ct_root->token_.accompanying_lexeme;
			break;

		case CrossTerminal::TYPE:
			assignSignature(ct_root->children_.at(0), entry_);
			if (ct_root->children_.size() == 2)
				assignSignature(ct_root->children_.at(1), entry_);
			break;

		case CrossTerminal::ARRAY:
			assignSignature(ct_root->children_.at(1), entry_);
			break;

		case CrossTerminal::BASIC_TYPE:
			switch (ct_root->children_.at(0)->node_type)
			{
				case CrossTerminal::IDENTIFIER_CLASS:
					entry_.class_identifier = ct_root->children_.at(0)->token_.accompanying_lexeme;
					break;
				case CrossTerminal::IDENTIFIER_PACKAGE:
					entry_.module_identifier = ct_root->children_.at(0)->token_.accompanying_lexeme;
					entry_.class_identifier = ct_root->children_.at(1)->token_.accompanying_lexeme;
					break;
				case CrossTerminal::PRIMITIVE_SIGNED:
				case CrossTerminal::PRIMITIVE_UNSIGNED:
					entry_.class_identifier = ct_root->children_.at(0)->token_.accompanying_lexeme;
					break;
				default:
					break;
			}
		case CrossTerminal::KEYWORD_CONST:
		case CrossTerminal::KEYWORD_PTR:
		case CrossTerminal::KEYWORD_REF:
			entry_.type_prefixes.emplace_back(ct_root->node_type);
			assignSignature(ct_root->children_.at(0), entry_);
		default:
			break;
	}
}

}

bool TableBuilder::runOn
(
	protocols::ConcreteSyntaxTree *ct_root,
	const std::string &qualified_name
)
{
	std::cout << ct_root->toString();
	sym::Module *mod_entry = new sym::Module;
	bool err_ = extractEntries(ct_root, *mod_entry);
	if (err_) return err_;
	return false;
}

bool TableBuilder::extractEntries
(
	protocols::ConcreteSyntaxTree *ct_root,
	sym::Module &mod_entry
)
{
	assert(ct_root != nullptr);
	sym::Entry *sym_entry = new sym::Entry;
	switch (ct_root->node_type)
	{
		case protocols::CrossTerminal::ENTER:
		{
			sym_entry->access_specifier = getAccessSpecifier(ct_root);
			sym_entry->object_access_specifier = getObjectSpecifier(ct_root);
			assignSignature(ct_root, *sym_entry);

			protocols::ConcreteSyntaxTree *ct_new = ct_root->children_.at(1)->children_.at(1);
			extractEntries(ct_new, mod_entry);
		}
		break;
		default:
		break;
	}
	std::cout << sym_entry->toString() << std::endl;
	return false;
}

}}
