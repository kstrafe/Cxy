def __init__(self, table):
	self._table = table
	self._state = 0

def transist(self, elet):
	compound = self._table[self._state][elet]
	self._state = compound[0]
	return compound[1]


