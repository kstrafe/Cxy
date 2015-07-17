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
#include "CrossTerminalTools.hpp"

namespace tul { namespace protocols { 

bool CrossTerminalTools::isKeyword(const CrossTerminal &ct_)
{
    switch (ct_)
    {
        case CrossTerminal::KEYWORD_ASSEMBLY: return true;
        case CrossTerminal::KEYWORD_CAST: return true;
        case CrossTerminal::KEYWORD_CONST: return true;
        case CrossTerminal::KEYWORD_DO: return true;
        case CrossTerminal::KEYWORD_ELSE: return true;
        case CrossTerminal::KEYWORD_FOR: return true;
        case CrossTerminal::KEYWORD_GLOBAL: return true;
        case CrossTerminal::KEYWORD_GLOCAL: return true;
        case CrossTerminal::KEYWORD_GOTO: return true;
        case CrossTerminal::KEYWORD_IF: return true;
        case CrossTerminal::KEYWORD_LABEL: return true;
        case CrossTerminal::KEYWORD_PRIVATE: return true;
        case CrossTerminal::KEYWORD_PTR: return true;
        case CrossTerminal::KEYWORD_PUBLIC: return true;
        case CrossTerminal::KEYWORD_PURE: return true;
        case CrossTerminal::KEYWORD_REF: return true;
        case CrossTerminal::KEYWORD_RESTRICTED: return true;
        case CrossTerminal::KEYWORD_RETURN: return true;
        case CrossTerminal::KEYWORD_VAR: return true;
        case CrossTerminal::KEYWORD_WHILE: return true;
        default: return false;
    }
}

}}
