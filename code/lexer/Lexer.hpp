#pragma once

#include "dependency/ActionGenerator.hpp"
#include "dependency/TokenGenerator.hpp"

#include "protocols/Token.hpp"
#include "protocols/Type.hpp"

#include <vector>

namespace tul
{
  namespace lexer
  {
    class Lexer
    {
    public:

      void insertCharacter(char character);
      std::vector<protocols::Token> &getTokenStack();

    private:

      template <typename F>
      bool isAnyOf(char a, F f)
      {
        return a == f;
      }

      template <typename F, typename ...Fs>
      bool isAnyOf(char a, F f, Fs ...fs)
      {
        return a == f || isAnyOf(a, fs...);
      }

      void identifyToken(protocols::Token &token);

      protocols::Type typify (char val);

      std::vector<protocols::Token> token_stack;

      dependency::ActionGenerator action_generator;
      dependency::TokenGenerator token_generator;

    };
  } // namespace lexer
} // namespace tul
