# -*- coding: utf-8 -*-
# Copyright © 2015 Kevin Robert Stravers
"""
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
"""

def computeAllCrossTerminals(productions):
	cross_terminals = {}
	for symbol_name in productions:
		for production in productions[symbol_name]:
			for element in production:
				cross_terminals[element] = None
	return cross_terminals


def computeFirstSet(symbol_name, productions, already_used, root):
	if symbol_name in already_used:
		raise AssertionError('Fatal error: symbol already used in FIRST set: \'' + symbol_name + '\' in expansion of \'' + root + '\'')
		sys.exit()
	first_set = []
	already_used.append(symbol_name)
	if symbol_name in productions:
		for production in productions[symbol_name]:
			for element in production:
				sub_first_set, is_element_epsilonable = computeFirstSet(element, productions, already_used, root + '->' + element)
				first_set.extend(sub_first_set)

				# If the element is epsilonable, that means we go to the next element as well
				if is_element_epsilonable:
					continue
				else:
					break
		return first_set, isEpsilonable(symbol_name, productions)
	else:
		return [symbol_name], False


def computeTerminals(productions):
	terminal_set = set()
	non_terminal_set = set()
	for non_terminal in productions:
		for results_ in productions[non_terminal]:
			for token_ in results_:
				first_set, epsilonable_ = computeFirstSet(token_, productions, [], non_terminal)
				for terminal_ in first_set:
					terminal_set.add(terminal_)
	cross_set = computeAllCrossTerminals(productions)
	for type_ in cross_set:
		if type_ not in terminal_set:
			if type_ not in non_terminal_set:
				non_terminal_set.add(type_)
	return terminal_set, non_terminal_set


def computeTransitions(symbol_name, productions):
	transition_set = {}
	transition_set[symbol_name] = {}
	for production in productions[symbol_name]:
		first_set = []
		for element in production:
			sub_first_set, is_element_epsilonable = computeFirstSet(element, productions, [], element)
			first_set.append(sub_first_set)
			if is_element_epsilonable:
				continue
			else:
				for elem in first_set:
					for sti in elem:
					    transition_set[symbol_name][sti] = production
				break
	return transition_set


def isEpsilonable(symbol_name, productions):
	if symbol_name in productions:
		if [] in productions[symbol_name]:
			return True
		else:
			return False
	return False


def generateTransitionMapCode(transition_set, productions):
	lines_ = []
	for symbol_ in transition_set:
		for look_ahead in transition_set[symbol_]:
			line_ = 'add(' + symbol_ + ', ' + look_ahead + ') {'
			products_ = []
			for product_ in transition_set[symbol_][look_ahead]:
				products_.append('cT(' + product_ + ')')
			line_ += ', '.join(products_)
			line_ += '});'
			lines_.append(line_)
		if isEpsilonable(symbol_, productions):
			lines_.append('eps(' + symbol_ + ');')
	lines_.sort()
	return '\n'.join(lines_)


def validateAllFirstSets(productions):
	for start_nonterminal in sorted(productions):
		items, eps = computeFirstSet(start_nonterminal, productions, [], start_nonterminal)
