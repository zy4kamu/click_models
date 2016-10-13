import os
import sys

items = []
with open(sys.argv[1]) as reader:
	for index, line in enumerate(reader):
		if index % 100000 == 0:
			print index
		user = line.split('\t')[0]
		if len(items) == 0 or user != items[-1]:
			items.append(user)
	pass
with open(sys.argv[2], 'w') as writer:
	for item in items:
		writer.write(str(item) + '\n')
	pass
		
