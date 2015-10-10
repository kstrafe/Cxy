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
#include "TreeBuilder.hpp"
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
		using namespace tul::tb;
		TreeBuilder builder_object;
		bool ret_val = true;
		char current = '\0';
		for (auto input_character : string)
		{
			if (builder_object.buildTree(input_character) == false)
			{
				ret_val = false;
				break;
			}
			current = input_character;
		}
		if (ret_val)
			ret_val = builder_object.endInput();
		if (ret_val == false && print_error_if_exists)
		{
			std::cout << "last char: " << current << std::endl;
			std::vector<std::string> expected = builder_object.getExpectedTokens();
			std::cout << "\nError: expected:\n";
		}
		return ret_val;
	}
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The API is quite simple. It is demonstrated and tested here.
////////////////////////////////////////////////////////////////////////////////
TEST_CASE("TreeBuilder must validate input", "[test-TreeBuilder]")
{
	using namespace tul::tb;

	SECTION("Attempting to validate a small program")
	{
		// Create an instance and feed characters into it.
		// Every time a character is fed the return value
		// tells you if the input is still valid.
		TreeBuilder builder_object;
		std::string input_string = "ClassName variable_name; String identifier_name; ";
		// Note the trailing space in the string. This causes the TreeBuilder to be able
		// To turn ; into a token, and thereby finish the input. If we left it out
		// The ; would merely be in a "current_working" state. Nothing would be invalidated,
		// It's just that we would not see it in the tree.
		for (auto input_character : input_string)
		{
			// Note that each character is validated, and aded to the tree if it completes
			// a token. Data structure:
			/*
				string: current building token. (characters are appended)
				TokenTree
				| --> TokenTree
							| --> TokenTree
				| --> TokenTree --> TokenTree

				The tokentree is a tree of tokens. Once the current building token
				is finished, it's appended to the tree by the internal parser.
			*/
			REQUIRE(builder_object.buildTree(input_character));
		}

		// Eventually, we may want to get that tree
		REQUIRE(builder_object.getConcreteSyntaxTree() != nullptr);
	}
	////////////////////////////////////////////////////////////////////////////////
	SECTION("Parse the start of a module: declarations. Some permutations.")
	{
		REQUIRE(validate(R"(
			1u b;
			1u a;
			1u c;
		)"));
		REQUIRE(validate(R"(
			1u a;
			1u c;
			1u b;
		)"));
		REQUIRE(validate(R"(
			1u c;
			1u b;
			1u a;
		)"));
	}
	////////////////////////////////////////////////////////////////////////////////
	SECTION("Parse the start of a module: declarations and assignments")
	{
		REQUIRE(validate(R"(
			1u b = 1;
			1u a = 2;
			1u c = 3;
		)"));
		REQUIRE(validate(R"(
			1u a(value: 4);
			1u c(value: 5);
			1u b(value: 6);
		)"));
		REQUIRE(validate(R"(
			1u c(value: 7);
			1u b = 8;
			1u a;
		)"));
	}
	////////////////////////////////////////////////////////////////////////////////
	SECTION("Check non-data class with expressions")
	{
		REQUIRE(validate("(:) enterProgram { }"));
		REQUIRE(validate("(:) enterProgram { a = \"Hi!\"; }"));
		REQUIRE(validate("(:) enterProgram { a = \"Hi!\"; b = 300; }"));
		REQUIRE(validate("(:) enterProgram { a = \"Hi!\"; b = 300; b_a =  b; }"));
		REQUIRE(validate("(:) enterProgram { a = \"Hi!\"; b = 300; b_a = 10 * b; }"));
		REQUIRE(validate("(:) enterProgram { b_a = 100 && 10 * b + 5 / 3 - another_identifier; }"));
		REQUIRE(validate("(:) enterProgram { b_a = 100 && 10 * b + 5 / 3 - another_identifier | \"Cool m8\"; }"));
		REQUIRE(validate("(:) enterProgram { b_a = sampleFunction()~a; }"));
		REQUIRE(validate("(:) enterProgram { b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a - another_identifier | \"Cool m8\"; }"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a - another_identifier | "Cool m8";
				q_a_d = 100 && 10 * b + 5 / 3 * sampleFunction()~a - another_identifier | "Cool m8";
				b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a - another_identifier | "Cool m8";
				b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a ^sampleFunction()~a - another_identifier | "Cool m8";
				var Type b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a ^sampleFunction()~a - another_identifier | "Cool m8";
				var 800u b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a ^sampleFunction()~a - another_identifier | "Cool m8";
				var 8s b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a ^sampleFunction()~a - another_identifier | "Cool m8";
				var AnotherClassName b_a = 100 > 5 && 10 * b + 5 / 3 * sampleFunction()~a ^sampleFunction()~a - another_identifier | "Cool m8";
				var AnotherClassName b_a = when (a) b else c;
			}
		)"));
		REQUIRE(false == validate("(:) enterProgram { var const ClassName ", false));
		REQUIRE(validate("(:) enterProgram { var const ptr ClassName alpha; }"));
		REQUIRE(false == validate("(:) enterProgram { var const const ClassName alpha; }", false));
		REQUIRE(validate("(:) enterProgram { var const ptr const ClassName alpha; }"));
	}
	////////////////////////////////////////////////////////////////////////////////
	SECTION("A class using different statement types")
	{
		REQUIRE(validate(R"(
			(:) _enterProgram
			{
				var 32u max = 1000;
				var 32u counter = 0;
				while (counter < 1000)
				{
					if (counter > 500 + 20)
					{ printSomething(); }
					for (const 33s cuda = 10; cuda < 20; ++cuda; --cuda;)
					{
						++global_var;
					}
				}
			}
		)"));
	}
	////////////////////////////////////////////////////////////////////////////////
	SECTION("Multiple function definitions")
	{
		////////////////////////////////////////////////////////////
		REQUIRE(validate(R"(
			32u variable = 0;

			(:) enterProgram
			{
				++variable;
			}

			(:) performCalculation
			{
				--variable;
			}
		)"));
		////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////
		REQUIRE(validate(R"(
			32u variable = 0;

			(:) enterProgram
			{
				++variable;
			}

			(:) performCalculation
			{
				--variable;
			}
		)"));
		////////////////////////////////////////////////////////////
	}
	////////////////////////////////////////////////////////////
	SECTION("Unary operators")
	{
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var 32u a = 1000;
				var ptr 32u b = $a;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var 32u a = 1000;
				var ptr 32u b = $a;
				++@b;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var 32u a = 1000;
				var ptr 32u b = $$a;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var 32u a = 1000;
				var ptr const 32u b = $$a;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var 32u a = 1000;
				var ptr const 32u b = $$a;
				var 32u c = @@b;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var 32u a = 1000;
				var ptr 32u b = $a;
				++ @b;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var 32u a = 1000;
				var ptr 32u b = $a;
				if (!! (@b > 100))
				{

				}
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var 32u a = 1000;
				var ptr 32u b = $a;
				if (!! (@b > 100))
				{

				}
			}

			(: : pure) funcTion
			{

			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var 32u a = 1000;
				var ptr 32u b = $a;
				if (!! (@b > 100))
				{

				}
			}

			(: : const) strToInt
			{}

			(: : const pure) funcTion
			{}


			(:: const) strToInte
			{}

			(:: const pure) funcTione
			{}
		)"));
	}
	////////////////////////////////////////////////////////////
	SECTION("Argument Lists")
	{
		REQUIRE(validate(R"(
			(32u a:32u b) myFunction
			{}
		)"));
		REQUIRE(validate(R"(
			(32u a, Class b:32u c) myFunction
			{}
		)"));
		REQUIRE(validate(R"(
			(32u a, Class b : Class c, 32u d) myFunction
			{}
		)"));
		REQUIRE(validate(R"(
			(32u a, 32u b : Class c, Class c2, 32u d, 32u e) myFunction
			{}
		)"));
		REQUIRE(validate(R"(
			([10, 32u] a, [10, 32u] b : Class c, Class c2, 32u d, 32u e) myFunction
			{}
		)"));
	}
	////////////////////////////////////////////////////////////
	SECTION("Array types")
	{
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var [3, 32u] arr;
				arr[1] = 200;
			}

			(:) anotherFunction
			{
				var [2, [3, [4, [5, 8u]]]] szt;
			}
		)"));
	}
	////////////////////////////////////////////////////////////
	SECTION("Putting variables in the middle is not allowed")
	{
		REQUIRE(false == validate(R"(
			(:) enterProgram
			{}

			32u var;
		)", false));
	}
	////////////////////////////////////////////////////////////
	SECTION("Type casting")
	{
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var 32u a = 100;
				var 16u b = cast(16u) { a } * 5;
			}
		)"));
	}
	////////////////////////////////////////////////////////////
	SECTION("Member expression")
	{
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var SomeClass a = SomeClass.createInstance()~instance;
				var SomeClass b = SomeClass.something()~ck;
				var SomeClass c = sys.Cool.something()~ck()~x[3 + 8];
				var Qs d = q.Qz.ctOr()~obj;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var Aa a = Aa();
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var a.Aa a = Aa();
				a = b;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				{

				}
				var a.Aa a = a.Aa();
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var a.Aa a = a.b.Aa();
			}
		)", false));
		REQUIRE(false == validate(R"(
			(:) enterProgram
			{
				var a.b.Aa a = a.Aa();
			}
		)", false));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				var sys.Dir dir(start: "/");
				var sml.String paths;
				while (dir.hasChild()~yes)
				{
					 paths += dir.getCurrent()~child[1 + f()~g[a.Aa.h()~j()~k[l]]];
				}

				a.Aa.g += f;
			}
		)"));
		REQUIRE(false == validate(R"(
			(:) enterProgram
			{
				a = b = c;
			}
		)", false));

		REQUIRE(validate(R"(
			(:) enterProgram
			{
				a[0][1][2];
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				a[0, 1, 2];
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				sys.StdOut.printLine(string: callChild())~alpha;
			}

			(23u nm, 23u top_kek : 800u kek) topKek
			{
				return nm: kek + 200;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				if (a)
					aAa();
				else if (b)
					bBb();
				else if (c)
					cCc();
				else
					dDd();
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				if (a)
				{  aAa();  }
				else if (b)
				{  bBb();  }
				else if (c)
				{  cCc();  }
				else
				{ dDd();   }
				if (e)
					what;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				if (a)
					if (b)
						what;
					else
						while (c)
							inside_while;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
							hack
				(
							assembly: "mov %eax, %ebx"
				);
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				static if (3)
				{
					a + b;
				}
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				static if (sys.Machine.memory_size)
				{
					a + b;
				}
				static else
				{
					static if (sys.Machine.processor_type)
					{
						a - b;
					}
					static else
					{
						a * b;
					}
				}
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				defer a * 4 + someStuff()~a;
				var Object object;
				object.initObject();
				defer
				{
					object.destroyObject();
				}
			}
		)"));
		REQUIRE(validate(R"(
			(:) enterProgram
			{
				static if (sys.Sys.argv[1])
				{
					var 32u x = computeSomething(in: 100, sec: 100 * 3)~value;
					sys.Out.printLn(val: x);
				}
			}

			(32u value : 64s in, 64s sec : const pure) computeSomething
			{
				var SomeClass object(arg: in);
				if (sec > 100)
				{
					defer object.doAbove();
					object.doSense();
				}
				return value: object.getVariable()~gotten;
			}
		)"));
	}
}
