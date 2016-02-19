def __init__(self, enum):
	self._enum = enum

def typify(self, character):
	v = ord(character)
	u = character
	if 65 <= v and v <= 90 or 97 <= v and v <= 122 or \
		v == 95 or 48 <= v and v <=57:
		return self._enum.ALPHA_DIGIT_OR_UNDERSCORE
	if u == '`':
		return self._enum.QUOTE_SYMBOL
	if u in ['(', ')', '[', ']', '{', '}']:
		return self._enum.GROUPING_SYMBOL
	elif u in [' ', '\n', '\r', '\f', '\v', '\t']:
		return self._enum.WHITESPACE
	elif v >= 128 or v in [0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 127]:
		return self._enum.UNKNOWN_CODE_POINT
	else:
		return self._enum.OTHER_SYMBOL
