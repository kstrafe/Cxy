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

#include <string>


namespace tul { namespace protocols {

class CrossTerminalTools
{
public:

	static bool isKeyword(const CrossTerminal &ct_);
	static bool isExpression(const CrossTerminal &ct_);
	static bool isUselessSymbol(const CrossTerminal &ct_);
	static std::string toString(protocols::CrossTerminal cross_terminal);
};

}}
