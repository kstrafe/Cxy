#include "KeywordMatcher.hpp"


namespace tul
{
	namespace lexer
	{
		namespace dependency
		{
			protocols::TokenType KeywordMatcher::getKeyword(const std::string &lexeme)
			{
				if (lexeme == "assembly") return protocols::TokenType::KEYWORD_ASSEMBLY;
				else if (lexeme == "do") return protocols::TokenType::KEYWORD_DO;
				else if (lexeme == "for") return protocols::TokenType::KEYWORD_FOR;
				else if (lexeme == "goto") return protocols::TokenType::KEYWORD_GOTO;
				else if (lexeme == "if") return protocols::TokenType::KEYWORD_IF;
				else if (lexeme == "label") return protocols::TokenType::KEYWORD_LABEL;
				else if (lexeme == "private") return protocols::TokenType::KEYWORD_PRIVATE;
				else if (lexeme == "public") return protocols::TokenType::KEYWORD_PUBLIC;
				else if (lexeme == "restricted") return protocols::TokenType::KEYWORD_RESTRICTED;
				else if (lexeme == "while") return protocols::TokenType::KEYWORD_WHILE;
				else return protocols::TokenType::UNIDENTIFIED;
			}
		}
	}
}