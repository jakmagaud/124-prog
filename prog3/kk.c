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
unsigned long arr[ARR_LEN];
unsigned long kk_buf[ARR_LEN];

void print_arr(int *arr);

/* MISCALLANEOUS FUNCTIONS */
int rng(int min, int max) {
	return rand() % (max + 1 - min) + min;
}

int prob_rng(void) {
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

int* rand_solution_p(void) {
	int* P = malloc(sizeof(int) * ARR_LEN);

	for (int i = 0; i < ARR_LEN; i++) 
		P[i] = rng(0,ARR_LEN);
	
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
int test_solution_s(int* sol) {
	int resid = 0;
	for (int i = 0; i < ARR_LEN; i++) {
		resid += sol[i] * arr[i];
	}
	return abs(resid);
}

int test_solution_p(int* sol) {
	int resid = 0;

	unsigned long* aPrime = calloc(1, sizeof(unsigned long) * ARR_LEN);
	for (int i = 0; i < ARR_LEN; i++)
		aPrime[sol[i]] += arr[i];

	return kk(aPrime);
}

/* KARMARKAR-KARP */
void get_max_inds(int* max_ind, int* next_max_ind, unsigned long* array) {
	int max = 0;
	int next_max = 0;
	*max_ind = *next_max_ind = -1;
	for (int i = 0; i < ARR_LEN; i++) {
		if (array[i] > max) {
			next_max = max;
			max = array[i];
			*next_max_ind = *max_ind;
			*max_ind = i;
		}
		else if (array[i] > next_max) {
			next_max = array[i];
			*next_max_ind = i;
		}
	}
}

int kk(unsigned long* array) {
	int* solution = malloc(sizeof(int) * ARR_LEN);
	memcpy(kk_buf, array, sizeof(unsigned long) * ARR_LEN);
	int resid = 0;
	int max_ind, next_max_ind;
	while (true) {
		get_max_inds(&max_ind, &next_max_ind, kk_buf);
		if (kk_buf[next_max_ind] == 0) {
			resid = kk_buf[max_ind];
			break;
		}
		resid = kk_buf[max_ind] - kk_buf[next_max_ind];
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
		free(random);
	}
	return current; 
}

int* hill_climb_s(void) {
	int* current = rand_solution_s();
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

		if (test_solution_s(tmp) < test_solution_s(current)) {
			memcpy(current, tmp, sizeof(int) * ARR_LEN);
		}
	}
	free(tmp);
	return current;
}

int* annealing_s(void) {
	int* current = rand_solution_s();
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
	free(current);
	free(tmp);
	return best;
}

/* PREPARTITIONING REPRESENTATION ALGORITHMS */
int* repeated_random_p(void) {
	int* current = rand_solution_p();
	for (int i = 0; i < MAX_ITER; i++) {
		int* random = rand_solution_p();
		if (test_solution_p(random) < test_solution_p(current)) {
			memcpy(current, random, sizeof(int) * ARR_LEN);
		}
		free(random);
	}
	return current; 
}

int* hill_climb_p(void) {
	int* current = rand_solution_p();
	int* tmp = malloc(sizeof(int) * ARR_LEN);

	for (int i = 0; i < MAX_ITER; i++) {
		memcpy(tmp, current, sizeof(int) * ARR_LEN);
		int index1 = rng(0, ARR_LEN);
		int index2 = rng(0, ARR_LEN);
		while (index1 == index2)
			index2 = rng(0, ARR_LEN);

		arr[index2] = arr[index1];
		int rnjesus = rng(0,1);
		if (rnjesus) {
			int index3 = rng(0, ARR_LEN);
			int index4 = rng(0, ARR_LEN);
			while (index3 == index4)
				index4 = rng(0, ARR_LEN);

			arr[index4] = arr[index3];
		} 

		if (test_solution_p(tmp) < test_solution_p(current)) {
			memcpy(current, tmp, sizeof(int) * ARR_LEN);
		}
	}
	print_arr(current);
	free(tmp);
	return current;
}

int* annealing_p(void) {
	int* current = rand_solution_p();
	int* tmp = malloc(sizeof(int) * ARR_LEN);
	int* best = malloc(sizeof(int) * ARR_LEN);

	for (int i = 0; i < MAX_ITER; i++) {
		memcpy(tmp, current, sizeof(int) * ARR_LEN);
		int index1 = rng(0, ARR_LEN);
		int index2 = rng(0, ARR_LEN);
		while (index1 == index2)
			index2 = rng(0, ARR_LEN);

		arr[index2] = arr[index1];
		int rnjesus = rng(0,1);
		if (rnjesus) {
			int index3 = rng(0, ARR_LEN);
			int index4 = rng(0, ARR_LEN);
			while (index3 == index4)
				index4 = rng(0, ARR_LEN);

			arr[index4] = arr[index3];
		} 

		if (test_solution_p(tmp) < test_solution_p(current))
			memcpy(current, tmp, sizeof(int) * ARR_LEN);
		else {
			double prob = pow(M_E, -1 * (test_solution_p(tmp) - test_solution_p(current))/cool_schedule(i));
			assert(prob <= 1 && prob >= 0.0);
			if (prob < prob_rng())
				memcpy(current, tmp, sizeof(int) * ARR_LEN);
		}

		if (test_solution_p(current) < test_solution_p(best))
			memcpy(best, current, sizeof(int) * ARR_LEN);
	}
	free(current);
	free(tmp);
	print_arr(best);
	return best;
}

void print_arr(int *arr) {
	for (int i = 0; i < ARR_LEN; i++)
		printf("%d ", arr[i]);

	printf("\n");
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
	print_arr((int*) arr);
	printf("Karmarkar-karp solution: %d\n", kk(arr));
	// printf("Standard representation solutions:\n");
	// int* solution = repeated_random_s();
	// printf("Repeated random: %d\n", test_solution_s(solution));
	// solution = hill_climb_s();
	// printf("Hill climb: %d\n", test_solution_s(solution));
	// solution = annealing_s();
	// printf("Annealing: %d\n", test_solution_s(solution));
	printf("Prepartition representation solutions:\n");
	int *solution = repeated_random_p();
	printf("Repeated random: %d\n", test_solution_p(solution));
	solution = hill_climb_p();
	printf("Hill climb: %d\n", test_solution_p(solution));
	// solution = annealing_p();
	// printf("Annealing: %d\n", test_solution_p(solution));
	return 0;
}
