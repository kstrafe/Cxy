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
#include "libraries/catch.hpp"
#include "protocols/SyntaxTree.hpp"
#include "SemanticAnalyzer.hpp"

#include <iostream>

TEST_CASE("Test the semantic analyzer", "[test-SemanticAnalyzer]")
{
	using namespace tul::protocols;
	using St = tul::protocols::SyntaxTree;

	// Various building procedures
	auto var = [](St *access, St *globality, St *type, St *declaration)
		-> St *
		{
			return new St(
				CrossTerminal::KEYWORD_VAR,
				{access, globality, type, declaration});
		};
	auto global = [](){ return new St(CrossTerminal::KEYWORD_GLOBAL); };
	auto tprivate = []() { return new St(CrossTerminal::KEYWORD_PRIVATE); };
	auto tpublic = []() { return new St(CrossTerminal::KEYWORD_PUBLIC); };
	auto trestricted = []() { return new St(CrossTerminal::KEYWORD_RESTRICTED); };
	auto type = [](St *modifier, St *type) { return new St(CrossTerminal::TYPE, {modifier, type}); };
	auto eps = [](){ return new St(CrossTerminal::EPSILONATE); };
	auto intu = [](std::size_t size){ St *a = new St(CrossTerminal::PRIMITIVE_UNSIGNED); a->setLexeme(std::to_string(size)); return a; };
	auto intuw = [](std::size_t size){ St *a = new St(CrossTerminal::PRIMITIVE_UNSIGNED_WRAPPED); a->setLexeme(std::to_string(size)); return a; };
	auto ints = [](std::size_t size){ St *a = new St(CrossTerminal::PRIMITIVE_SIGNED); a->setLexeme(std::to_string(size)); return a; };
	auto intsw = [](std::size_t size){ St *a = new St(CrossTerminal::PRIMITIVE_SIGNED_WRAPPED); a->setLexeme(std::to_string(size)); return a; };
	auto namelist = [](const std::string &varname,
		St *expression, St *namelist2)
		{
			St *a = new St(CrossTerminal::DATA_NAMES, {expression, namelist2});
			a->setLexeme(varname);
			return a;
		};
	auto enter = [](St *first, St *tail) { return new St(CrossTerminal::ENTER, {first, tail}); };
	auto and_expr = [](St *first, St *second) { return new St(CrossTerminal::AND_EXPRESSION, {first, second}); };
	auto unary_expr = [](St *symbol, St *underexpression) { return new St(CrossTerminal::UNARY_EXPRESSION, {symbol, underexpression}); };
	auto dereference = []() { return new St(CrossTerminal::SYMBOL_APETAIL); };
	auto minus = []() { return new St(CrossTerminal::SYMBOL_MINUS); };
	auto addressof = []() { return new St(CrossTerminal::SYMBOL_DOLLAR); };
	auto constaddressof = []() { return new St(CrossTerminal::SYMBOL_DOLLAR__DOLLAR); };
	auto integer = [](std::string literal) { return (new St(CrossTerminal::INTEGER_LITERAL))->setLexeme(literal); };
	tul::sem::SemanticAnalyzer semant;
	SECTION("Basic validation")
	{
		using namespace tul::protocols;
		/*
			public global var 32u test = 100, abc = 10000;
		*/
		std::unique_ptr<SyntaxTree> top;
		top.reset(enter(
			var(
				tpublic(),
				global(),
				type(eps(), intu(32)),
				namelist(
					"test",
					integer("100"),
					namelist(
						"abc",
						unary_expr(
							addressof(),
							integer("10000")),
						eps()
					))),
			eps()));
		REQUIRE(semant.checkTree(top.get()));
		/* var 32u a = 1; */
		top.reset(enter(
			var(
				eps(), eps(), type(eps(), intu(32)),
				namelist( "a", unary_expr( minus(),
					integer("1")), eps())), eps()));
		REQUIRE(semant.checkTree(top.get()));
		top.reset(enter(
			var(
				eps(),
				eps(),
				type(eps(), intu(32)),
				namelist( "a",
					and_expr(
						integer("2"),
						integer("1")),
					eps())),
			eps()));
		REQUIRE(semant.checkTree(top.get()));
	}
}
