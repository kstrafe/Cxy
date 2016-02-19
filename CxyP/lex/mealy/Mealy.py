def __init__(self, table):
	self._table = table
	self._state = 0
	self._size = len(table)

def transist(self, elet):
	compound = self._table[self._state * self._size + elet]
	self._state = compound[0]
	return compound[1]


