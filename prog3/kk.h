#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ARR_LEN 5
#define MAX_ITER 25000

double cool_schedule(int i);
int rng(int min, int max);
int prob_rng(void);

int* rand_solution(void);
//int test_solution(int* sol);

void get_max_inds(int* max_ind, int* next_max_ind, unsigned long* array);
int kk(unsigned long* array);
