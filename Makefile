CC = gcc -std=c99 -g -ggdb3

fibmake: randmst.o
	$(CC) -o randmst randmst.c

clean:
	rm -f randmst *.o *~