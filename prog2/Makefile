CC = gcc -std=c99 -g -ggdb3

strassen: strassen.o 
	$(CC) -o strassen strassen.o 

strassen.o: strassen.c strassen.h
	$(CC) -c strassen.c

clean:
	rm -f strassen *.o *~