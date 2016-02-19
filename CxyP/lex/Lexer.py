_position_counter = count.PositionCounter()

def __init__(self, entrytypes):
	self._entrytypes = entrytypes
	self._typifier = typify.Typifier(entrytypes)

def lex(self, character):
	self._position_counter.count(character)
	# print(self.mealymachine.transist(0))
	chartype = self._typifier.typify(character)
	print(self._entrytypes.name[chartype])




