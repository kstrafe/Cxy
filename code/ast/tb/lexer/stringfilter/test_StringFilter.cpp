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
#include "StringFilter.hpp"

#include <iostream>
#include <string>

namespace
{
	std::string construct(const std::string &string)
	{
		using namespace tul::stringfilter;
		StringFilter filter;
		std::string b;
		for (char a : string)
		{
			filter.push(a);
			while (filter.available())
				b.push_back(filter.pop());
		}
		filter.end();
		while (filter.available())
			b.push_back(filter.pop());
		return b;
	}
}

TEST_CASE("Test if the string filter works correctly", "[test-StringFilter]")
{
	#define test(x, y) REQUIRE(construct(x) == y)
	SECTION("Compare different strings")
	{
		REQUIRE(construct("a") == "a");
		test(R"('\n')", "\"\n\"");
		test(R"('\t')", "\"\t\"");
		test(R"('\r')", "\"\r\"");
		test(R"("\r")", "\"\r\"");
		test(R"(`\r`)", R"("\r")");
		test(R"(`\r``)", R"("\r`)");
		test(R"(`"`)", R"("""")");
		test(R"("\"")", R"("""")");
		test(R"('\"')", R"("""")");
		test(R"(`\"`)", R"("\""")");
	}
	SECTION("Escape inside different quotes")
	{
		test(R"("")", R"("")");
		test(R"(``)", R"("")");
		test(R"('"')", R"("""")");
		test(R"("'")", R"("'")");
		test(R"("""")", R"("""")");
		test(R"(`'`)", R"("'")");
		test(R"(`"`)", R"("""")");
		test(R"(`"``"`)", R"("""`""")");
		test(R"(`e " f`;)", R"("e "" f";)");
		test(R"()", R"()");
	}
	#undef test
}
