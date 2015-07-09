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


def isEpsilonable(symbol_name, productions):
    if symbol_name in productions:
        if [] in productions[symbol_name]:
            return True
        else:
            return False
    return False


def computeFirstSet(symbol_name, productions, already_used=[]):
    if symbol_name in already_used:
        raise AssertionError('Fatal error: symbol already used in FIRST set: \'' + symbol_name + '\'')
        sys.exit()
    first_set = []
    already_used.append(symbol_name)
    if symbol_name in productions:
        for production in productions[symbol_name]:
            for element in production:
                sub_first_set, is_element_epsilonable = computeFirstSet(element, productions, already_used)
                first_set.extend(sub_first_set)

                # If the element is epsilonable, that means we go to the next element as well
                if is_element_epsilonable:
                    continue
                else:
                    break
        return first_set, isEpsilonable(symbol_name, productions)
    else:
        return [symbol_name], False


def validateAllFirstSets(productions):
    for start_nonterminal in sorted(productions):
        items, eps = computeFirstSet(start_nonterminal, productions, [])


def computeTransitions(symbol_name, productions):
    transition_set = {}
    transition_set[symbol_name] = {}
    for production in productions[symbol_name]:
        first_set = []
        for element in production:
            sub_first_set, is_element_epsilonable = computeFirstSet(element, productions, [])
            first_set.append(sub_first_set)
            if is_element_epsilonable:
                continue
            else:
                for elem in first_set:
                    for sti in elem:
                        transition_set[symbol_name][sti] = production
                break
    return transition_set


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
    return '\n'.join(lines_)
