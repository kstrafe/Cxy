import util

_position_counter = count.PositionCounter()

def __init__(self, entrytypes):
	self._entrytypes = entrytypes
	self._typifier = typify.Typifier(entrytypes)
	self._akt = self.util.Enum.generate('N', 'E', 'P', 'PTG', 'TAPTG', 'TA', 'TAP', 'TRP', 'TR', 'TRPTG', 'TSP', 'TSPTG', 'TS')
	akt = self._akt
	table = [
	# ALPHA_DIGIT_OR_UNDERSCORE GROUPING_SYMBOL QUOTE_SYMBOL OTHER_SYMBOL  UNKNOWN_CODE_POINT WHITESPACE
		[1, akt.P],               [0, akt.PTG],   [2, akt.N],  [4, akt.P],   [0, akt.E],        [0, akt.N],  # 0
		[1, akt.P],               [0, akt.TAPTG], [2, akt.TA], [4, akt.TAP], [0, akt.E],        [0, akt.TA], # 1
		[2, akt.P],               [2, akt.P],     [3, akt.N],  [2, akt.P],   [2, akt.P],        [2, akt.P],  # 2
		[1, akt.TRP],             [0, akt.TRPTG], [2, akt.P],  [4, akt.TRP], [0, akt.E],        [0, akt.TR], # 3
		[1, akt.TSP],             [0, akt.TSPTG], [2, akt.TS], [4, akt.P],   [0, akt.E],        [0, akt.TS]  # 4
	]
	self._mealy = mealy.Mealy(table, 6)

def lex(self, character):
	self._position_counter.count(character)
	chartype = self._typifier.typify(character)
	# print(self._entrytypes.name[chartype])
	action = self._mealy.transist(chartype)
	print(self._akt.name[action])






