def sortTable(productions, terminals, prepend):
	temp = """{p},\n"""
	string = ''
	seen = ['ENTER']
	to_process = ['ENTER']
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
					if j not in terminals:
						if j not in seen:
							group.insert(0, j)
							seen.append(j)
			string += '],\n'
	return 'productions = {{\n{x}\n}}'.format(x=prepend(string))
