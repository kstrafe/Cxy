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
#include "libraries/catch.hpp"
#include "protocols/CrossTerminal.hpp"
#include "protocols/CrossTerminalTools.hpp"

#include "protocols/ParseReturn.hpp"

#include <cassert>
#include <iostream>


////////////////////////////////////////////////////////////////////////////////
// This is only a small utility validator... if you want to see how
// TreeBuilder works just go down to "TEST_CASE"
////////////////////////////////////////////////////////////////////////////////
namespace
{
	bool validate(const std::string &string, bool print_error_if_exists = true)
	{
		tul::ast::AbstractSyntaxTreeGenerator builder_object;
		bool ret_val = true;
		for (auto input_character : string)
		{
			if (builder_object.buildTree(input_character) == false)
			{
				ret_val = false;
				break;
			}
		}
		if (ret_val)
			ret_val = builder_object.endInput();
		if (ret_val)
			builder_object.generateSemantics("root.Main");
		return ret_val;
	}
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The API is quite simple. It is demonstrated and tested here.
////////////////////////////////////////////////////////////////////////////////
TEST_CASE("AbstractSyntaxTreeGenerator must validate the grammar.", "[test-AbstractSyntaxTreeGenerator]")
{
	#define doValidation(x) REQUIRE(validate(x))
	doValidation(R"(
		(:) enter
		{
			sml.Out.print(:"Hello World!");
			sml.Out.print(:"Hello Error!");
			sml.Out.print(:sys.Std.readString()~string);
		}

		(32u a : 32u b) someFunction
		{
			return a: b;
		}
	)");

	doValidation(R"(
		var 32u a = 3, b = 100;
		var ptr (sml.String ret : sml.String inp) ptr2_fun;

		(:) enter
		{
			var 64s some64s; // Allocate on the stack
			var sml.String some_string;
		}

		(32u a, 32u d, my.Class x: 32u b, 64s c, 64s y) someFunction
		{
			var 64s some64s;
			var sml.String some_string;
		}
	)");

	doValidation(R"(
		(: ((:) b :) a) doStuff
		{}
	)");

	doValidation(R"(
		(:) doStuff
		{}
		grant Aa {
			(:) a;
		}
		public var Aa a;
	)");
	doValidation(R"(
		alias {
			st = stiti;
			dude = what;
			Smh = what.Cool;
		}
		grant Aa {
			(:) >;
			var ptr 1u dude;
		}
		grant EBBA [EBBA + DERP, ptr [3, 8u]];
	)");
	#undef doValidation
}
