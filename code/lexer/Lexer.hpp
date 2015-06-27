#pragma once

#include "dependency/ActionGenerator.hpp"
#include "dependency/PositionCounter.hpp"
#include "dependency/TokenGenerator.hpp"

#include "protocols/Token.hpp"
#include "protocols/EntryType.hpp"

#include <vector>

namespace tul
{
  namespace lexer
  {
    class Lexer
    {
    public:

      bool insertCharacter(char in_character);
      std::vector<protocols::Token> &getTokenStack();

    private:

      template <typename F>
      bool isAnyOf(char a_, F f_)
      {
        return a_ == f_;
      }

      template <typename F, typename ...Fs>
      bool isAnyOf(char a_, F f_, Fs ...fs_)
      {
        return a_ == f_ || isAnyOf(a_, fs_...);
      }

      protocols::TokenType getKeyword(const std::string &test_lexeme) const;

      void identifyToken(protocols::Token &token);

      protocols::EntryType typify (char val_);

      std::vector<protocols::Token> token_stack;

      dependency::ActionGenerator action_generator;
      dependency::PositionCounter position_counter;
      dependency::TokenGenerator token_generator;

    };
  } // namespace lexer
} // namespace tul
