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
namespace tul { namespace sma {

  class SemanticAnalyzer
  {
  public:

    SemanticAnalyzer();
    ~SemanticAnalyzer();

    struct Access
    {
      enum class AccessSpecifier
      {
        PRIVATE,
        PUBLIC,
        RESTRICTED,
      };
    };

    struct Type
    {
      std::string
        type_modifier,
        basic_type;
    };

    struct DataDeclaration
    {
      Access access_;
      Type type_;
      std::string name_;
    };

    struct MethodSignature
    {
      std::vector<std::pair<Type, std::string>>
        in_args,
        out_args;
    };

    struct MethodSymbolTable
    {
      std::vector<MethodSignature> signature_;
      std::string method_name;
    };

    struct ModuleSymbolTable
    {
      std::vector<DataDeclaration> module_scope_data;
      MethodSymbolTable method_symtab;
    };

    std::map<std::string,
  };

}}
