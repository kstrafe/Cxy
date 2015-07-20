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
#include "protocols/Action.hpp"
#include "protocols/EntryType.hpp"

#include <cstddef>


namespace tul { namespace actgen {

/**
  The action generator receives character types. Based on this information,
  it advances a state machine (a mealy machine) and returns the action that
  the edge of the transition is annotated with.

  For example, on the input:
  aa b c
  it will first say "push, push, transfer" on 'aa ' (note the space). This
  means that a lexeme storage is allocated for each push and the character
  is put onto that storage. The transfer says that the token is finished.

  This class solely concerns itself with providing information about what
  action to take.
*/
template <typename MACHINE>
class ActionGenerator
{
public:

  ActionGenerator()
  {
    /*
      n = no action
      e = error, stray unknown outside a string literal
      p = push current OTHER_SYMBOL on to the current token
      tx = transfer something of type x
      tg = transfer a GROUPING_SYMBOL token
      tr = transfer a string token
      ts = transfer a OTHER_SYMBOL token
      ta = transfer an alnumu token
      txp = transfer x and then push
      ptx = push and then transfer
    */
    mealy_machine.setTransitionTable(action_table);
  }

  protocols::Action computeAction(protocols::EntryType with_type)
  {
    return mealy_machine.transist(with_type, 6);
  }



private:

  MACHINE mealy_machine;
  // static const MACHINE<std::size_t, protocols::Action, protocols::EntryType>::Compound action_table[];

  static constexpr const typename  MACHINE::Compound action_table[]
  { /*    ALPHA_DIGIT_OR_UNDERSCORE  GROUPING_SYMBOL     QUOTE_SYMBOL     OTHER_SYMBOL      UNKNOWN_CODE_POINT  WHITESPACE  */
    /*0*/ {1, protocols::Action::P},            {0, protocols::Action::PTG},   {2, protocols::Action::N},  {4, protocols::Action::P},   {0, protocols::Action::E},     {0, protocols::Action::N},
    /*1*/ {1, protocols::Action::P},            {0, protocols::Action::TAPTG}, {2, protocols::Action::TA}, {4, protocols::Action::TAP}, {0, protocols::Action::E},     {0, protocols::Action::TA},
    /*2*/ {2, protocols::Action::P},            {2, protocols::Action::P},     {3, protocols::Action::N},  {2, protocols::Action::P},   {2, protocols::Action::P},     {2, protocols::Action::P},
    /*3*/ {1, protocols::Action::TRP},          {0, protocols::Action::TRPTG}, {2, protocols::Action::P},  {4, protocols::Action::TRP}, {0, protocols::Action::E},     {0, protocols::Action::TR},
    /*4*/ {1, protocols::Action::TSP},          {0, protocols::Action::TSPTG}, {2, protocols::Action::TS}, {4, protocols::Action::P},   {0, protocols::Action::E},     {0, protocols::Action::TS}
    // Remember to set the transist value in computeprotocols::Action to the amount of columns here.
  };

};

template <typename MACHINE>
constexpr const typename MACHINE::Compound ActionGenerator<MACHINE>::action_table[];

}}
