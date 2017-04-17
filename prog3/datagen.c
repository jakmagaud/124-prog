#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

int main(void) {
	srand(time(NULL));
	for (int i = 0; i < 100; i++) {
		unsigned long random = (unsigned long) (rand() % ((unsigned long) pow(10,10)) + 1);
		printf("%lu\n", random);
	}
}