# -*- coding: utf-8 -*-
# Copyright © 2015 Kevin Robert Stravers
notice = """This file is part of Cxy Compiler Reference Implementation (Cxy CRI).

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

"""

import re

def commentOut(string):
	"""Prepend a comment in front of every line"""
	return re.sub(r'^|\n', r'\n' + '# ', string)

def sortTable(productions, terminals, prepend):
	temp = """{p},\n"""
	string = ''
	seen = ['ENTER']
	to_process = ['ENTER']
	references = {'ENTER': 0}
	# Iterate all elements, if they're in the terminal set, just ignore them.
	group = ['ENTER']
	while group:
		to_process = group
		group = []
		string += 60 * '#' + '\n'
		while to_process:
			processing = to_process.pop()
			start = productions[processing]
			start.sort()
			string += "'" + processing + "': [\n"
			for i in start:
				string += '\t' + temp.format(p=str(i))
				for j in i:
					if j in references:
						references[j] += 1
					else:
						references[j] = 1
					if j not in terminals:
						if j not in seen:
							group.insert(0, j)
							seen.append(j)
			string += '],\n'
	statistics = ''
	for i in sorted(references):
		statistics += '# ' + i + ': ' + str(references[i]) + '\n'
	to_ret = '{stats}\nproductions = {{\n{x}\n}}'.format(x=prepend(string),
	                                                   stats=statistics)
	return commentOut(notice) + to_ret
