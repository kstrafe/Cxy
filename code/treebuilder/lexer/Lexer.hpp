// Copyright © 2015 Kevin Robert Stravers
/*
This file is part of Unnamed-Language Compiler Reference Implementation (ULCRI).

ULCRI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ULCRI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ULCRI.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
#include "actgen/ActionGenerator.hpp"
#include "actgen/TokenGenerator.hpp"
#include "dependency/Mealy.hpp"
#include "dependency/PositionCounter.hpp"
#include "protocols/EntryType.hpp"
#include "protocols/Token.hpp"

#include <vector>


namespace tul { namespace lexer {

/**
  This class has as a task to create tokens from character data. It processes
  one character per call (using insertCharacter) and pushes it through
  various stages. This may or may not update the token stack. This stack
  (or rather, vector), may be extracted afterwards in order to process it
  somewhere else. The good news is that extracting the token stack can be
  done via std::move or std::swap, such that this class easily becomes
  a component in a multithreaded environment:
    Thread one: process 10 mb of characters
    Thread two: wait, then process ~1 million characters
    Thread one: fetches another 10 mb of characters and works on that.
    Thread two: keeps processing the tokens.
*/
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

  actgen::ActionGenerator<dependency::Mealy<std::size_t, protocols::Action, protocols::EntryType>> action_generator;
  dependency::PositionCounter position_counter;
  dependency::TokenGenerator token_generator;

};

}}
