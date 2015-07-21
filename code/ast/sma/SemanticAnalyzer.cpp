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
#include "SemanticAnalyzer.hpp"

#include <iostream>


namespace tul { namespace sma {

SemanticAnalyzer::SemanticAnalyzer()
{
  sym::ModuleTable mod_tab;
  mod_tab.qualified_name = "root.Main";
  module_symtab.program_symtab.insert(mod_tab);
}

SemanticAnalyzer::~SemanticAnalyzer()
{
  sym::ModuleTable mod_tab;
  mod_tab.qualified_name = "root.Main";
  std::set<sym::ModuleTable>::iterator it = module_symtab.program_symtab.find(mod_tab);
  for (const sym::MethodTable &method_tab : it->method_symtab)
  {
    std::cout << "Registered function: " << method_tab.method_name << std::endl;
  }
}

bool SemanticAnalyzer::runOn(protocols::ConcreteSyntaxTree *ct_root)
{
  if
  (
    ct_root->node_type == protocols::CrossTerminal::ENTER
    || ct_root->node_type == protocols::CrossTerminal::DECL_OR_FUNC
    || ct_root->node_type == protocols::CrossTerminal::FUNCTION_LIST
  )
  {
    for (protocols::ConcreteSyntaxTree *ct_child : ct_root->children_)
    {
      runOn(ct_child);
    }
  }
  else if (ct_root->node_type == protocols::CrossTerminal::FUNCTION_DEFINITION)
  {
    sym::ModuleTable mod_tab;
    mod_tab.qualified_name = "root.Main";
    std::set<sym::ModuleTable>::iterator it = module_symtab.program_symtab.find(mod_tab);
    sym::MethodTable met_tab;
    met_tab.method_name = ct_root->children_.at(1)->token_.accompanying_lexeme;
    std::set<sym::MethodTable> &m_tab = const_cast<std::set<sym::MethodTable> &> (it->method_symtab);
    m_tab.insert(met_tab);
  }
  return false;
}

}}
