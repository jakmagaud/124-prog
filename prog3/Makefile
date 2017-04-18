CC = gcc -std=c99 -g -ggdb3

signs: kk_signs.o 
	$(CC) -o kk_signs kk_signs.o  

signs.o: kk_signs.c  kk.h
	$(CC) -c kk_signs.c 

prepartition: kk_prepartition.o
	$(CC) -o kk_prepartition kk_prepartition.o

prepartition.o: kk_prepartition.c
	$(CC) -c kk_prepartition.c
clean:
	rm -f kk* nums*.txt *.o *~