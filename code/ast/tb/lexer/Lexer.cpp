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
#include "Lexer.hpp"
#include "dependency/KeywordMatcher.hpp"
#include "dependency/SymbolMatcher.hpp"
#include "protocols/Token.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <vector>


namespace tul { namespace lexer {

bool Lexer::insertCharacter(char character)
{
	comment_filter.push(character);
	while (comment_filter.available())
	{
		char top = comment_filter.pop();
		if (! insertCharacterAfterComments(top))
			return false;
	}
	position_counter.countCharacter(character);
	return true;
}


bool Lexer::insertCharacterAfterComments(char character)
{
	protocols::EntryType type_of_character = typify(character);
	protocols::Action action_to_perform = action_generator.computeAction(type_of_character);
	{
		std::size_t amount_of_new_tokens = token_generator.consumeCharacter(character, action_to_perform);
		bool error = amount_of_new_tokens == std::numeric_limits<std::size_t>::max();
		if (error) return false;
		for (
			std::size_t new_token = getTokenStack().size() - amount_of_new_tokens
			; new_token < getTokenStack().size();
			++new_token
		) {
			protocols::Token &curr_token = getTokenStack()[new_token];
			identifyToken(curr_token);

			{
				protocols::Token potential_next {
					0, 0, protocols::EntryType::OTHER_SYMBOL,
					protocols::TokenType::UNIDENTIFIED, ""};

				while (
					curr_token.token_type == protocols::TokenType::UNIDENTIFIED
					&& curr_token.entry_type == protocols::EntryType::OTHER_SYMBOL
				) {
					if (curr_token.accompanying_lexeme.empty()) {
						return false;
					}
					// Take the last character off...
					// Try and match with a shorter symbol.
					// Really don't like this.
					char last = curr_token.accompanying_lexeme.back();
					curr_token.accompanying_lexeme.erase(curr_token.accompanying_lexeme.size() - 1);
					potential_next.accompanying_lexeme.insert(potential_next.accompanying_lexeme.begin(), last);
					identifyToken(curr_token);
				}
				if (potential_next.accompanying_lexeme.empty() == false)
					getTokenStack().insert(getTokenStack().begin() + new_token + 1, potential_next);
			}
			getTokenStack()[new_token].column_number = position_counter.getCurrentColumnNumber();
			getTokenStack()[new_token].line_number = position_counter.getCurrentLineNumber();
		}
	}
	return true;
}


std::vector<protocols::Token> &Lexer::getTokenStack()
{
	return token_generator.getTokenStack();
}


const std::vector<protocols::Token> &Lexer::getTokenStack() const
{
	return token_generator.getTokenStack();
}


protocols::TokenType Lexer::getKeyword(const std::string &test_lexeme) const
{
	return dependency::KeywordMatcher::getKeyword(test_lexeme);
}


void Lexer::identifyToken(protocols::Token &input_token)
{
	if (input_token.accompanying_lexeme.size() == 0)
		throw std::string("Lexer::identifyToken: Zero-sized");

	identifyTokenAfterStrippingUnderscores(input_token);
}


void Lexer::identifyTokenAfterStrippingUnderscores(protocols::Token &input_token)
{
	using namespace protocols;

	std::string &lex = input_token.accompanying_lexeme;

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
	auto is_keyword = [&]() -> bool {return getKeyword(lex) != TokenType::UNIDENTIFIED;};
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
			input_token.token_type = getKeyword(lex);
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
		input_token.token_type = dependency::SymbolMatcher::getSymbol(lex);
	else
		input_token.token_type = TokenType::UNIDENTIFIED;
}


protocols::EntryType Lexer::typify (char val)
{
	using namespace protocols;
	const constexpr unsigned char UTF_8_LIMIT = 128;
	if ((65 <= val && val <= 90) || (97 <= val && val <= 122) || val == 95 || (48 <= val && val <= 57))
		return EntryType::ALPHA_DIGIT_OR_UNDERSCORE;
	if (val == '"')
		return EntryType::QUOTE_SYMBOL;
	if (isAnyOf(val, '(', ')', '{', '}', '[', ']'))
		return EntryType::GROUPING_SYMBOL;
	else if (isAnyOf(val, ' ', '\n', '\r', '\f', '\v', '\t'))
		return EntryType::WHITESPACE;
	else if (static_cast<unsigned char>(val) >= UTF_8_LIMIT || isAnyOf(val, 0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 127))
		return EntryType::UNKNOWN_CODE_POINT;
	else
		return EntryType::OTHER_SYMBOL;
}


std::size_t Lexer::getColumn() const
{
	return position_counter.getCurrentColumnNumber();
}


std::size_t Lexer::getLine() const
{
	return position_counter.getCurrentLineNumber();
}

}}
