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
#include <cstddef>

namespace action {

enum class Action { N, E, P, PTG, TAPTG, TA, TAP, TRP, TR, TRPTG, TSP, TSPTG, TS  };

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

template <typename Machine, typename EntryType>
class ActionGenerator {
public:

	ActionGenerator() {
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

	Action computeAction(EntryType with_type) {
		return mealy_machine.transist(with_type, 6);
	}

private:

	Machine mealy_machine;
	// static const Machine<std::size_t, Action, EntryType>::Compound action_table[];

	static constexpr const typename  Machine::Compound action_table[] {
		/*    ALPHA_DIGIT_OR_UNDERSCORE  GROUPING_SYMBOL     QUOTE_SYMBOL     OTHER_SYMBOL      UNKNOWN_CODE_POINT  WHITESPACE  */
		/*0*/ {1, Action::P},            {0, Action::PTG},   {2, Action::N},  {4, Action::P},   {0, Action::E},     {0, Action::N},
		/*1*/ {1, Action::P},            {0, Action::TAPTG}, {2, Action::TA}, {4, Action::TAP}, {0, Action::E},     {0, Action::TA},
		/*2*/ {2, Action::P},            {2, Action::P},     {3, Action::N},  {2, Action::P},   {2, Action::P},     {2, Action::P},
		/*3*/ {1, Action::TRP},          {0, Action::TRPTG}, {2, Action::P},  {4, Action::TRP}, {0, Action::E},     {0, Action::TR},
		/*4*/ {1, Action::TSP},          {0, Action::TSPTG}, {2, Action::TS}, {4, Action::P},   {0, Action::E},     {0, Action::TS}
		// Remember to set the transist value in compute Action to the amount of columns here.
	};

};

template <typename Machine, typename EntryType>
constexpr const typename Machine::Compound ActionGenerator<Machine, EntryType>::action_table[];

}
