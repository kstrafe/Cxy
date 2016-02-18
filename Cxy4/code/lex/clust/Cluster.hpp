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
#include <algorithm>
#include <vector>

namespace clust {

	template <typename TokenStack, typename EntryType, typename TokenType, typename Matcher, typename SymbolMatcher>
	class Cluster {
	public:

		static void cluster(TokenStack &from, TokenStack &to) {
			while (from.empty() == false) {
				auto &token = from.front();
				to.emplace_back(from.front());
				from.erase(from.begin());
			}
		}

		template <typename Token>
		static void identifyToken(Token &input_token)
		{

			std::string &lex = input_token.accompanying_lexeme;

			Matcher keyword;

			auto all_digit = [&]() -> bool {return std::all_of(lex.cbegin(), lex.cend(), [](char in_character) -> bool {return std::isdigit(in_character);});};
			// auto all_lower = [&input_token]() -> bool {return std::all_of(lex.cbegin(), lex.cend(), [](char in_character) -> bool {return std::islower(in_character);});};
			auto all_upper = [&]() -> bool {return std::all_of(lex.cbegin(), lex.cend(), [](char in_character) -> bool {return std::isupper(in_character);});};

			auto any_digit = [&]() -> bool {return std::any_of(lex.cbegin(), lex.cend(), [](char in_character) -> bool {return std::isdigit(in_character);});};
			auto any_lower = [&]() -> bool {return std::any_of(lex.cbegin(), lex.cend(), [](char in_character) -> bool {return std::islower(in_character);});};
			auto any_underscore = [&]() -> bool {return std::any_of(lex.cbegin(), lex.cend(), [](char in_character) -> bool {return in_character == '_'; });};
			auto any_upper = [&]() -> bool {return std::any_of(lex.cbegin(), lex.cend(), [](char in_character) -> bool {return std::isupper(in_character);});};

			auto begins_with_lowercase = [&]() -> bool {return std::islower(lex.front());};
			auto begins_with_uppercase = [&]() -> bool {return std::isupper(lex.front());};

			auto ends_with_lowercase = [&]() -> bool {return std::islower(lex.back());};

			auto fromEnd = [&](int n, char x) -> bool {return *(lex.cend() - n) == x;};

			auto is_class_identifier = [&]() -> bool {return begins_with_uppercase() && any_underscore() == false && ends_with_lowercase();};
			auto is_constexpr_identifier = [&]() -> bool {return all_upper() && any_underscore() == false && any_lower() == false; };
			auto is_enumeration_identifier = [&]() -> bool {return begins_with_uppercase() && any_underscore() == true && any_lower() == false; };
			auto is_function_identifier = [&]() -> bool {return begins_with_lowercase() && any_underscore() == false && ends_with_lowercase() && any_upper();};
			auto is_keyword = [&]() -> bool {return keyword.getKeyword(lex) != TokenType::UNIDENTIFIED;};
			auto is_number_literal = [&]() -> bool {return all_digit();};
			auto is_package_identifier = [&]() -> bool {return any_underscore() == false && any_upper() == false && any_lower() && begins_with_lowercase();};
			auto is_primitive_signed = [&]() -> bool {return lex.back() == 's' && std::all_of(lex.cbegin(), lex.cend() - 1, [](char in_character) -> bool {return std::isdigit(in_character);});};
			auto is_primitive_unsigned = [&]() -> bool {return lex.back() == 'u' && std::all_of(lex.cbegin(), lex.cend() - 1, [](char in_character) -> bool {return std::isdigit(in_character);});};
			auto is_primitive_signed_wrapped = [&]() -> bool {return fromEnd(1, 'o') && fromEnd(2, 's') && std::all_of(lex.cbegin(), lex.cend() - 2, [](char in_character) -> bool {return std::isdigit(in_character);});};
			auto is_primitive_unsigned_wrapped = [&]() -> bool {return fromEnd(1, 'o') && fromEnd(2, 'u') && std::all_of(lex.cbegin(), lex.cend() - 2, [](char in_character) -> bool {return std::isdigit(in_character);});};
			auto is_variable_identifier = [&]() -> bool {return any_underscore() && any_upper() == false && any_lower();};

			/*
				ClassNamesAreLikeThisNoUnderScoreEndWithSmallCharacter
				ClassName.ENUMERATIVENAMESLIKETHIS, THEY_ALLOW_UNDERSCORES
				Make enums be anonymous
				Package vs object clashes in names? Shouldn't be a prob.
			*/
			if (input_token.entry_type == EntryType::ALPHA_DIGIT_OR_UNDERSCORE)
			{
				if (is_class_identifier())
					 input_token.token_type = TokenType::IDENTIFIER_CLASS;
				else if (is_enumeration_identifier())
					input_token.token_type = TokenType::IDENTIFIER_ENUMERATION;
				else if (is_constexpr_identifier())
					input_token.token_type = TokenType::IDENTIFIER_CONSTEXPR;
				else if (is_function_identifier())
					input_token.token_type = TokenType::IDENTIFIER_SUBROUTINE;
				else if (is_number_literal())
					input_token.token_type = TokenType::INTEGER_LITERAL;
				else if (is_keyword())
					input_token.token_type = keyword->getKeyword(lex);
				// Fold this with identifier variable.
				else if (is_package_identifier())
					input_token.token_type = TokenType::IDENTIFIER_PACKAGE;
				else if (is_primitive_signed())
					input_token.token_type = TokenType::PRIMITIVE_SIGNED;
				else if (is_primitive_unsigned())
					input_token.token_type = TokenType::PRIMITIVE_UNSIGNED;
				else if (is_primitive_signed_wrapped())
					input_token.token_type = TokenType::PRIMITIVE_SIGNED_WRAPPED;
				else if (is_primitive_unsigned_wrapped())
					input_token.token_type = TokenType::PRIMITIVE_UNSIGNED_WRAPPED;
				else if (is_variable_identifier())
					input_token.token_type = TokenType::IDENTIFIER_VARIABLE;
				else
					input_token.token_type = TokenType::UNIDENTIFIED;
			}
			else if (input_token.entry_type == EntryType::GROUPING_SYMBOL)
			{
				switch (lex.at(0))
				#define caze(enum_token_type) input_token.token_type = TokenType::enum_token_type; break;
				{
					case '(': caze(GROUPER_LEFT_PARENTHESIS)
					case ')': caze(GROUPER_RIGHT_PARENTHESIS)
					case '[': caze(GROUPER_LEFT_BRACKET)
					case ']': caze(GROUPER_RIGHT_BRACKET)
					case '{': caze(GROUPER_LEFT_BRACE)
					case '}': caze(GROUPER_RIGHT_BRACE)
					default: break;
				}
				#undef caze
			}
			else if (input_token.entry_type == EntryType::QUOTE_SYMBOL)
				input_token.token_type = TokenType::STRING;
			else if (input_token.entry_type == EntryType::OTHER_SYMBOL)
				input_token.token_type = SymbolMatcher::getSymbol(lex);
			else
				input_token.token_type = TokenType::UNIDENTIFIED;
		}

	};

}
