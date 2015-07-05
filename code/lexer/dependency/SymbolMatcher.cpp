#include "SymbolMatcher.hpp"


namespace tul
{
	namespace lexer
	{
		namespace dependency
		{
			protocols::TokenType SymbolMatcher::getSymbol(const std::string &lexeme)
			{
				using namespace protocols;
				if (lexeme == "++") return protocols::TokenType::SYMBOL_PLUS__PLUS;
				else if (lexeme == "--") return protocols::TokenType::SYMBOL_MINUS__MINUS;
				else if (lexeme == "**") return protocols::TokenType::SYMBOL_STAR__STAR;
				else if (lexeme == "&&") return protocols::TokenType::SYMBOL_AMPERSAND__AMPERSAND;
				else if (lexeme == "||") return protocols::TokenType::SYMBOL_BAR__BAR;
				else if (lexeme == "==") return protocols::TokenType::SYMBOL_EQUAL__EQUAL;
				else if (lexeme == "..") return protocols::TokenType::SYMBOL_DOT__DOT;
				else if (lexeme == "::") return protocols::TokenType::SYMBOL_COLON__COLON;
				else if (lexeme == "<<") return protocols::TokenType::SYMBOL_LESS_THAN__LESS_THAN;
				else if (lexeme == ">>") return protocols::TokenType::SYMBOL_GREATER_THAN__GREATER_THAN;
				else if (lexeme == "+") return protocols::TokenType::SYMBOL_PLUS;
				else if (lexeme == "-") return protocols::TokenType::SYMBOL_MINUS;
				else if (lexeme == "*") return protocols::TokenType::SYMBOL_STAR;
				else if (lexeme == "/") return protocols::TokenType::SYMBOL_FORWARD_SLASH;
				else if (lexeme == "\\") return protocols::TokenType::SYMBOL_BACKWARD_SLASH;
				else if (lexeme == "&") return protocols::TokenType::SYMBOL_AMPERSAND;
				else if (lexeme == "|") return protocols::TokenType::SYMBOL_BAR;
				else if (lexeme == "^") return protocols::TokenType::SYMBOL_CARET;
				else if (lexeme == "!") return protocols::TokenType::SYMBOL_EXCLAMATION_MARK;
				else if (lexeme == "=") return protocols::TokenType::SYMBOL_EQUAL;
				else if (lexeme == ".") return protocols::TokenType::SYMBOL_DOT;
				else if (lexeme == ",") return protocols::TokenType::SYMBOL_COMMA;
				else if (lexeme == ":") return protocols::TokenType::SYMBOL_COLON;
				else if (lexeme == ";") return protocols::TokenType::SYMBOL_SEMICOLON;
				else if (lexeme == "<") return protocols::TokenType::SYMBOL_LESS_THAN;
				else if (lexeme == ">") return protocols::TokenType::SYMBOL_GREATER_THAN;
				else return protocols::TokenType::UNIDENTIFIED;
			}
		}
	}
}