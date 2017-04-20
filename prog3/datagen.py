import random
import sys

for i in range(int(sys.argv[1])):
	f = open('data/nums' + str(i) + '.txt', 'w')

	for _ in range (100):
		num = random.randint(1,pow(10,10))
		f.write(str(num) + '\n')

	print "finished writing"
