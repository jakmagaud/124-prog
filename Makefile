CC = gcc -std=c99 -g -ggdb3

randmst: randmst.o disjoint.o
	$(CC) -o randmst randmst.o disjoint.o

randmst.o: randmst.c randmst.h
	$(CC) -c randmst.c

disjoint.o: disjoint.c randmst.h
	$(CC) -c disjoint.c

clean:
	rm -f randmst disjoint *.o *~
