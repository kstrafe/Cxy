current_line = 1
current_column = 1
utf_bytes_left = 0

def count(self, character):
	print(ord(character), self.utf_bytes_left, character)
	if self.utf_bytes_left > 0:
		self.utf_bytes_left -= 1
		if self.utf_bytes_left == 0:
			self.current_column += 1
		return
	character = ord(character)
	if character & 0b10000000:
		if character & 0b01000000:
			if character & 0b00100000:
				if character & 0b00010000:
					if character & 0b00001000:
						if character & 0b00000100:
							self.utf_bytes_left = 5
						else:
							self.utf_bytes_left = 4
					else:
						self.utf_bytes_left = 3
				else:
					self.utf_bytes_left = 2
			else:
				self.utf_bytes_left = 1
		else:
			self.utf_bytes_left = 0
			print("UTF Handing error")
	else:
		if character == ord('\n'):
			self.current_line += 1
			self.current_column = 1
		else:
			self.current_column += 1

def getCurrentColumn(self):
	return self.current_column

def getCurrentLine(self):
	return self.current_line
