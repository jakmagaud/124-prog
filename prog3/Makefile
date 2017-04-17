CC = gcc -std=c99 -g -ggdb3

kk: kk.o 
	$(CC) -o kk kk.o 

kk.o: kk.c kk.h
	$(CC) -c kk.c

clean:
	rm -f kk nums*.txt *.o *~