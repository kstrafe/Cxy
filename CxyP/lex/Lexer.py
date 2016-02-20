import util
_position_counter = count.PositionCounter()

def __init__(self, entrytypes):
	self._entrytypes = entrytypes
	self._typifier = typify.Typifier(entrytypes)
	self._akt = util.Enum.generate('N', 'E', 'P', 'PTG', 'TAPTG', 'TA', 'TAP', 'TRP', 'TR', 'TRPTG', 'TSP', 'TSPTG', 'TS')
	akt = self._akt
	table = act.ActionTable.getTable(self._akt)
	self._mealy = mealy.Mealy(table)
	toktype = util.Enum.generate('UNIDENTIFIED')
	self._cluster = act.Cluster(akt, entrytypes, toktype)

def lex(self, character):
	self._position_counter.count(character)
	chartype = self._typifier.typify(character)
	# print(self._entrytypes.name[chartype])
	action = self._mealy.transist(chartype)
	print(character, self._akt.name[action], self._entrytypes.name[chartype])
	a = self._cluster.consume(character, action)
	for i in range(a):
		element = self._cluster.pop()
		print(element, 'is a token')
		print(match.Symbol.match(element[4]))
	# Next up: create value
