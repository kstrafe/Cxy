def generate(*values):
	values = dict(zip(values, range(len(values))))
	enums = type('Enum', (), values)
	enums.name = {v: k for k, v in values.items()}
	return enums
