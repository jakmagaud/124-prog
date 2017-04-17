import random

f = open('nums.txt', 'w')

for _ in range (100):
	num = random.randint(1,pow(10,10))
	f.write(str(num) + '\n')

print "finished writing"
