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
#include <vector>

#include "action/ActionGenerator.hpp"
#include "action/TokenGenerator.hpp"
#include "count/PositionCounter.hpp"
#include "mealy/Mealy.hpp"
#include "string/StringFilter.hpp"

namespace lex {

	template <typename EntryType, typename Token, typename TokenType, typename Typifier>
	class Lexer {

	public:
		Lexer() { }
		~Lexer() { }

		void insertCharacter(char in) {
			position_counter.countCharacter(in);
			/*
			can_continue = filterComments(in)
			if (can_continue) {
			*/
			string_filter.push(in);
			while (string_filter.available()) {
				in = string_filter.pop();
				auto chartype = Typifier::typify(in);
				auto action = action_gen.computeAction(chartype);
				token_gen.consumeCharacter(in, action);
			}
			for (auto &a : token_gen.getTokenStack()) {
				ready_tokens.push_back(a);
			}
			token_gen.getTokenStack().clear();
			/*
				cluster(in)
				processClusters()
			}
			*/
		}

		bool hasToken() const {
			return ready_tokens.size() > 0;
		}

		Token popToken() {
			auto back = ready_tokens.back();
			ready_tokens.pop_back();
			return back;
		}

	private:

		std::vector<Token> ready_tokens;
		count::PositionCounter position_counter;
		action::ActionGenerator<mealy::Mealy
			<std::size_t, action::Action, EntryType>,
			EntryType> action_gen;
		action::TokenGenerator<action::Action, EntryType, Token, TokenType> token_gen;
		string::StringFilter string_filter;

	};

}
