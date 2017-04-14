#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include "kk.h"

int rng(int min, int max) {
	return rand() % (max + 1 - min) + min;
}

solution* rand_solution(void) {
	solution* solution = malloc(sizeof(solution));
	solution->arr = malloc(sizeof(unsigned int) * ARR_LEN);
	solution->signs = malloc(sizeof(unsigned int) * ARR_LEN);

	for (int i = 0; i < ARR_LEN; i++) {
		solution->signs[i] = rng(0,1);
		if (solution->signs[i] == 0)
			solution->signs[i] = -1;
	}
	return solution;
}

int test_solution(solution* sol) {
	int resid = 0;
	for (int i = 0; i < ARR_LEN; i++) {
		resid += sol->signs[i] * sol->arr[i];
	}
	return resid;
}

solution* repeated_random(void) {
	solution* current = rand_solution();
	for (int i = 0; i < MAX_ITER; i++) {
		solution* random = rand_solution();
		if (test_solution(random) < test_solution(current))
			current = random;
	}
	return current; 
}

solution* hill_climb(void) {
	return NULL;
}

void read_file_data(char* fname, unsigned int* ret) {
	FILE* file = fopen(fname, "r");
	for (int i = 0; i < ARR_LEN; i++) {
		fscanf(file, "%d", &ret[i]);
	}
	fclose(file);
}

int main(int argc, char** argv) {
	srand(time(NULL));
	unsigned int* ret = malloc(sizeof(unsigned int) * ARR_LEN);

	/*
	solution* test = malloc(sizeof(solution));
	test->arr = malloc(sizeof(unsigned int) * 5);
	test->signs = malloc(sizeof(unsigned int) * 5);
	test->arr[0] = 10; test->arr[1] = 8; test->arr[2] = 7; test->arr[3] = 6; test->arr[4] = 5;
	test->signs[0] = -1; test->signs[1] = 1; test->signs[2] = 1; test->signs[3] = -1; test->signs[4] = 1;
	int resid = test_solution(test);
	printf("%d\n", resid);
	*/

	char* fname = argv[1];
	read_file_data(fname, ret);
	return 0;
}
