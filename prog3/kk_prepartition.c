#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "kk.h"

int test_solution_p(int* sol) {
	int resid = 0;

	unsigned long aPrime[ARR_LEN] = {0};
	for (int i = 0; i < ARR_LEN; i++)
		aPrime[sol[i]] += arr[i];

	return kk(aPrime);
}

int* repeated_random_p(void) {
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

int* hill_climb_p(void) {
	int* current = rand_solution();
	int* tmp = malloc(sizeof(int) * ARR_LEN);

	for (int i = 0; i < MAX_ITER; i++) {
		memcpy(tmp, current, sizeof(int) * ARR_LEN);
		int index1 = rng(0, ARR_LEN);
		int index2 = rng(0, ARR_LEN);
		while (index1 == index2 || arr[index1] == arr[index2])
			index2 = rng(0, ARR_LEN);

		arr[index2] = arr[index1];
		int rnjesus = rng(0,1);
		if (rnjesus) {
			int index3 = rng(0, ARR_LEN);
			int index4 = rng(0, ARR_LEN);
			while (index3 == index4 || arr[index3] == arr[index4])
				index4 = rng(0, ARR_LEN);

			arr[index4] = arr[index3];
		} 

		if (test_solution(tmp) < test_solution(current)) {
			memcpy(current, tmp, sizeof(int) * ARR_LEN);
		}
	}
	//free(tmp);
	return current;
}

int* annealing_p(void) {
	int* current = rand_solution();
	int* tmp = malloc(sizeof(int) * ARR_LEN);
	int* best = malloc(sizeof(int) * ARR_LEN);

	for (int i = 0; i < MAX_ITER; i++) {
		memcpy(tmp, current, sizeof(int) * ARR_LEN);
		int index1 = rng(0, ARR_LEN);
		int index2 = rng(0, ARR_LEN);
		while (index1 == index2 || arr[index1] == arr[index2])
			index2 = rng(0, ARR_LEN);

		arr[index2] = arr[index1];
		int rnjesus = rng(0,1);
		if (rnjesus) {
			int index3 = rng(0, ARR_LEN);
			int index4 = rng(0, ARR_LEN);
			while (index3 == index4 || arr[index3] == arr[index4])
				index4 = rng(0, ARR_LEN);

			arr[index4] = arr[index3];
		} 

		if (test_solution(tmp) < test_solution(current))
			memcpy(current, tmp, sizeof(int) * ARR_LEN);
		else {
			double prob = pow(M_E, -1 * (test_solution(tmp) - test_solution(current))/cool_schedule(i));
			assert(prob <= 1 && prob >= 0.0);
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
