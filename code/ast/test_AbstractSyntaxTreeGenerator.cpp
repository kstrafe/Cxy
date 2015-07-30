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

	REQUIRE(validate(R"(
		public (:) enterProgram
		{
			sys.Std.printOut(str_: "Hello World!");
			sys.Std.printErr(str_: "Hello Error!");
			sys.Std.printOut(str_: sys.Std.readString()~string_);
		}

		private (32u a_ : 32u b_) someFunction
		{
			return a_: b_;
		}
	)"));

	REQUIRE(validate(R"(
		global public 32u a_ = 3, b_ = 100;

		public (:) enterProgram
		{
			var 64s some_64s; // Allocate on the stack
			var sml.String some_string;
		}

		private (32u a_, d_, my.Class x_: 32u b_, 64s c_, y_) someFunction
		{
			var 64s some_64s;
			var sml.String some_string;
		}
	)"));
	REQUIRE(validate(R"()"));
}
