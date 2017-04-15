#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include "kk.h"

int arr[ARR_LEN];

int rng(int min, int max) {
	return rand() % (max + 1 - min) + min;
}

int prob_rng(void) {
	return (double) rand() / (double) ((unsigned) RAND_MAX + 1);
}

int* rand_solution(void) {
	int* signs = malloc(sizeof(int) * ARR_LEN);

	for (int i = 0; i < ARR_LEN; i++) {
		signs[i] = rng(0,1);
		if (signs[i] == 0)
			signs[i] = -1;
	}
	return signs;
}

int test_solution(int* sol) {
	int resid = 0;
	for (int i = 0; i < ARR_LEN; i++) {
		resid += sol[i] * arr[i];
	}
	return abs(resid);
}

int* repeated_random(void) {
	int* current = rand_solution();
	for (int i = 0; i < MAX_ITER; i++) {
		int* random = rand_solution();
		if (test_solution(random) < test_solution(current)) {
			memcpy(current, random, sizeof(int) * ARR_LEN);
		}
		//free(random);
	}
	return current; 
}

int* hill_climb(void) {
	int* current = rand_solution();
	int* tmp = malloc(sizeof(int) * ARR_LEN);

	for (int i = 0; i < MAX_ITER; i++) {
		memcpy(tmp, current, sizeof(int) * ARR_LEN);
		int index1 = rng(0, ARR_LEN);
		int index2 = rng(0, ARR_LEN);
		while (index1 == index2)
			index2 = rng(0, ARR_LEN);

		current[index1] *= -1;
		int rnjesus = rng(0,1);
		if (rnjesus) {
			current[index2] *= -1;
		}

		if (test_solution(tmp) < test_solution(current)) {
			memcpy(current, tmp, sizeof(int) * ARR_LEN);
		}
	}
	//free(tmp);
	return current;
}

double cool_schedule(int i) {
	return pow(10, 10) * pow(0.8, floor(i / 300));
}

int* annealing(void) {
	int* current = rand_solution();
	int* tmp = malloc(sizeof(int) * ARR_LEN);
	int* best = malloc(sizeof(int) * ARR_LEN);

	for (int i = 0; i < MAX_ITER; i++) {
		memcpy(tmp, current, sizeof(int) * ARR_LEN);
		int index1 = rng(0, ARR_LEN);
		int index2 = rng(0, ARR_LEN);
		while (index1 == index2)
			index2 = rng(0, ARR_LEN);

		current[index1] *= -1;
		int rnjesus = rng(0,1);
		if (rnjesus) {
			current[index2] *= -1;
		}

		if (test_solution(tmp) < test_solution(current))
			memcpy(current, tmp, sizeof(int) * ARR_LEN);
		else {
			double prob = pow(M_E, -1 * (test_solution(current) - test_solution(tmp))/cool_schedule(i));
			if (prob < prob_rng())
				memcpy(current, tmp, sizeof(int) * ARR_LEN);
		}

		if (test_solution(current) < test_solution(best))
			memcpy(best, current, sizeof(int) * ARR_LEN);
	}
	//free(current);
	//free(tmp);
	return best;
}

void read_file_data(char* fname) {
	FILE* file = fopen(fname, "r");
	for (int i = 0; i < ARR_LEN; i++) {
		fscanf(file, "%d", &arr[i]);
	}
	fclose(file);
}

int main(int argc, char** argv) {
	srand(time(NULL));
	struct timeval t0;
    struct timeval t1;
    // gettimeofday(&t0, 0);
	// gettimeofday(&t1, 0);
	// long elapsed = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec;
	// printf("Operation took %ld microseconds \n", elapsed);

	char* fname = argv[1];
	read_file_data(fname);
	int* solution = repeated_random();
	printf("%d\n", test_solution(solution));
	solution = hill_climb();
	printf("%d\n", test_solution(solution));
	solution = annealing();
	printf("%d\n", test_solution(solution));
	return 0;
}
