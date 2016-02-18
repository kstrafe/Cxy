// Copyright © 2015-2016 Kevin Robert Stravers
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

namespace datru {

	enum class EntryType {
		ALPHA_DIGIT_OR_UNDERSCORE,
		GROUPING_SYMBOL, QUOTE_SYMBOL,
		OTHER_SYMBOL,
		UNKNOWN_CODE_POINT,
		WHITESPACE
	};

	template <typename F>
	bool isAnyOf(char a, F f) {
		return a == f;
	}

	template <typename F, typename ...Fs>
	bool isAnyOf(char a, F f, Fs ...fs) {
		return a == f || isAnyOf(a, fs...);
	}

	EntryType typify(char in) {
		const constexpr unsigned char UTF_8_LIMIT = 128;
		if ((65 <= in && in <= 90) || (97 <= in && in <= 122) || in == 95 || (48 <= in && in <= 57))
			return EntryType::ALPHA_DIGIT_OR_UNDERSCORE;
		if (in == '`')
			return EntryType::QUOTE_SYMBOL;
		if (isAnyOf(in, '(', ')', '{', '}', '[', ']'))
			return EntryType::GROUPING_SYMBOL;
		else if (isAnyOf(in, ' ', '\n', '\r', '\f', '\v', '\t'))
			return EntryType::WHITESPACE;
		else if (static_cast<unsigned char>(in) >= UTF_8_LIMIT || isAnyOf(in, 0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 127))
			return EntryType::UNKNOWN_CODE_POINT;
		else
			return EntryType::OTHER_SYMBOL;
	}

}
