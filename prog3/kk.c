#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "kk.h"

/* GLOBAL VARIABLES */
long arr[ARR_LEN];
long kk_buf[ARR_LEN];

void print_arr(int *arr);

/* MISCALLANEOUS FUNCTIONS */
int rng(int min, int max) {
	return rand() % (max + 1 - min) + min;
}

double prob_rng(void) {
	return (double) rand() / (double) ((unsigned) RAND_MAX + 1);
}

double cool_schedule(int i) {
	return pow(10, 10) * pow(0.8, floor(i / 300));
}

int* rand_solution_s(void) {
	int* signs = malloc(sizeof(int) * ARR_LEN);

	for (int i = 0; i < ARR_LEN; i++) {
		signs[i] = rng(0,1);
		if (signs[i] == 0)
			signs[i] = -1;
	}
	return signs;
}

long* rand_solution_p(void) {
	long* P = malloc(sizeof(long) * ARR_LEN);

	for (int i = 0; i < ARR_LEN; i++) 
		P[i] = (long) rng(1,ARR_LEN);
	
	return P;
}

void read_file_data(char* fname) {
	FILE* file = fopen(fname, "r");
	for (int i = 0; i < ARR_LEN; i++) {
		fscanf(file, "%ld", &arr[i]);
	}
	fclose(file);
}

/* TEST FUNCTIONS */
long test_solution_s(int* sol) {
	long resid = 0;
	for (int i = 0; i < ARR_LEN; i++) {
		resid += sol[i] * arr[i];
	}
	return labs(resid);
}

long test_solution_p(long* sol) {
	long* aPrime = calloc(1, sizeof(long) * ARR_LEN);
	for (int i = 0; i < ARR_LEN; i++)
		aPrime[sol[i]-1] += arr[i];

	return kk(aPrime);
}

/* KARMARKAR-KARP */
void find_max(long* array, int* max_ind, int ind) {
	long max = -1;
	*max_ind = 0;
	for (int i = 0; i < ARR_LEN; i++) {
		if (i == ind)
			continue;
		if (array[i] > max) {
			max = array[i];
			*max_ind = i;
		}
	}
}

long kk(long* array) {
	memcpy(kk_buf, array, sizeof(long) * ARR_LEN);
	long resid = 0;
	int max_ind, next_max_ind;
	while (true) {
		find_max(kk_buf, &max_ind, -1);
		find_max(kk_buf, &next_max_ind, max_ind);
		if (kk_buf[next_max_ind] == 0) {
			resid = kk_buf[max_ind];
			break;
		}
		resid = labs(kk_buf[max_ind] - kk_buf[next_max_ind]);
		kk_buf[next_max_ind] = 0;
		kk_buf[max_ind] = resid;
	}
	return resid;
}

/* STANDARD REPRESENTATION ALGORITHMS */
int* repeated_random_s(void) {
	int* current = rand_solution_s();
	for (int i = 0; i < MAX_ITER; i++) {
		int* random = rand_solution_s();
		if (test_solution_s(random) < test_solution_s(current)) {
			memcpy(current, random, sizeof(int) * ARR_LEN);
		}
		//free(random);
	}
	return current; 
}

int* hill_climb_s(void) {
	int* current = rand_solution_s();
	int* tmp = malloc(sizeof(int) * ARR_LEN);

	for (int i = 0; i < MAX_ITER; i++) {
		memcpy(tmp, current, sizeof(int) * ARR_LEN);
		int index1 = rng(0, ARR_LEN - 1);
		int index2 = rng(0, ARR_LEN - 1);
		while (index1 == index2)
			index2 = rng(0, ARR_LEN - 1);

		current[index1] *= -1;
		int rnjesus = rng(0,1);
		if (rnjesus) {
			current[index2] *= -1;
		}

		if (test_solution_s(tmp) < test_solution_s(current)) {
			memcpy(current, tmp, sizeof(int) * ARR_LEN);
		}
	}
	//free(tmp);
	return current;
}

int* annealing_s(void) {
	int* current = rand_solution_s();
	int* tmp = malloc(sizeof(int) * ARR_LEN);
	int* best = malloc(sizeof(int) * ARR_LEN);
	memcpy(best, current, sizeof(int) * ARR_LEN);

	for (int i = 0; i < MAX_ITER; i++) {
		memcpy(tmp, current, sizeof(int) * ARR_LEN);
		int index1 = rng(0, ARR_LEN - 1);
		int index2 = rng(0, ARR_LEN - 1);
		while (index1 == index2)
			index2 = rng(0, ARR_LEN - 1);

		current[index1] *= -1;
		int rnjesus = rng(0,1);
		if (rnjesus) {
			current[index2] *= -1;
		}

		if (test_solution_s(tmp) < test_solution_s(current))
			memcpy(current, tmp, sizeof(int) * ARR_LEN);
		else {
			double prob = pow(M_E, -1 * (test_solution_s(tmp) - test_solution_s(current))/cool_schedule(i));
			assert(prob <= 1 && prob >= 0.0);
			if (prob < prob_rng())
				memcpy(current, tmp, sizeof(int) * ARR_LEN);
		}

		if (test_solution_s(current) < test_solution_s(best))
			memcpy(best, current, sizeof(int) * ARR_LEN);
	}
	//free(current);
	//free(tmp);
	return best;
}

/* PREPARTITIONING REPRESENTATION ALGORITHMS */
long* repeated_random_p(void) {
	long* current = rand_solution_p();
	for (int i = 0; i < MAX_ITER; i++) {
		long* random = rand_solution_p();
		if (test_solution_p(random) < test_solution_p(current)) {
			memcpy(current, random, sizeof(long) * ARR_LEN);
		}
	}
	//free(random);
	return current; 
}

long* hill_climb_p(void) {
	long* current = rand_solution_p();
	long* tmp = malloc(sizeof(long) * ARR_LEN);

	for (int i = 0; i < MAX_ITER; i++) {
		memcpy(tmp, current, sizeof(long) * ARR_LEN);
		int index1 = rng(1, ARR_LEN);
		int index2 = rng(1, ARR_LEN);
		while (index1 == index2 || current[index1] == index2)
			index2 = rng(1, ARR_LEN);
		assert(index1 != index2 && current[index1] != index2);
		current[index1 - 1] = index2;

		if (test_solution_p(tmp) < test_solution_p(current)) {
			memcpy(current, tmp, sizeof(long) * ARR_LEN);
		}
	}
	//free(tmp);
	return current;
}

long* annealing_p(void) {
	long* current = rand_solution_p();
	long* tmp = malloc(sizeof(long) * ARR_LEN);
	long* best = malloc(sizeof(long) * ARR_LEN);
	memcpy(best, current, sizeof(long) * ARR_LEN);

	for (int i = 0; i < MAX_ITER; i++) {
		memcpy(tmp, current, sizeof(long) * ARR_LEN);
		int index1 = rng(1, ARR_LEN);
		int index2 = rng(1, ARR_LEN);
		while (index1 == index2 || current[index1] == index2)
			index2 = rng(1, ARR_LEN);
		assert(index1 != index2 && current[index1] != index2);
		current[index1 - 1] = index2;

		if (test_solution_p(tmp) < test_solution_p(current))
			memcpy(current, tmp, sizeof(long) * ARR_LEN);
		else {
			double prob = pow(M_E, -1 * (test_solution_p(tmp) - test_solution_p(current))/cool_schedule(i));
			assert(prob <= 1 && prob >= 0.0);
			if (prob < prob_rng())
				memcpy(current, tmp, sizeof(long) * ARR_LEN);
		}

		if (test_solution_p(current) < test_solution_p(best))
			memcpy(best, current, sizeof(long) * ARR_LEN);
	}
	//free(current);
	//free(tmp);
	return best;
}

int main(int argc, char** argv) {
	srand(69);
	struct timeval t0;
    struct timeval t1;
    // gettimeofday(&t0, 0);
	// gettimeofday(&t1, 0);
	// long elapsed = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec;
	// printf("Operation took %ld microseconds \n", elapsed);

	char* fname = argv[1];
	read_file_data(fname);

	// long* test = malloc(sizeof(long) * 5); test[0] = 1; test[1] = 2; test[2] = 2; test[3] = 4; test[4] = 5;
	// arr[0] = 10; arr[1] = 8; arr[2] = 7; arr[3] = 6; arr[4] = 5;
	// printf("%ld\n", test_solution_p(test));
	printf("Karmarkar-karp solution: %ld\n", kk(arr));
	int* solution = repeated_random_s();
	printf("Repeated random: %ld\n", test_solution_s(solution));
	solution = hill_climb_s();
	printf("Hill climb: %ld\n", test_solution_s(solution));
	solution = annealing_s();
	printf("Annealing: %ld\n", test_solution_s(solution));

	long* solution2 = repeated_random_p();
	printf("Prepartition repeated random: %ld\n", test_solution_p(solution2));
	solution2 = hill_climb_p();
	printf("Prepartition hill climb: %ld\n", test_solution_p(solution2));
	solution2 = annealing_p();
	printf("Prepartition annealing: %ld\n", test_solution_p(solution2));
	return 0;
}
