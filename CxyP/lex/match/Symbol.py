def match(characters):
	def check(string):
		return "".join(characters).startswith(string)
	if len(characters) >= 2:
		if check('+='):
			return ['aa']
		if check('-='):
			return ['aa']
		if check('*='):
			return ['aa']
		if check('/='):
			return ['aa']
	if len(characters) == 1:
		pass

	return []
