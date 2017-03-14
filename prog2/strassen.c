#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "strassen.h"

vector* create_vector(int len) {
	vector* vec = malloc(sizeof(vector));
	if (len < INIT_VEC_LEN)
		len = INIT_VEC_LEN;
	vec->buf = malloc(len * sizeof(int));
	vec->len = 0;
	vec->max_len = len;
	return vec;
}

void free_vector(vector* vec) {
	assert(vec != NULL);
	if (vec->buf != NULL)
		free(vec->buf);
	free(vec);
}

int vector_insert(vector* vec, int i) {
	if (vec->len >= vec->max_len) {
		vec->max_len *= 2;
		vec->buf = realloc(vec->buf, vec->max_len * sizeof(int));
	}
	vec->buf[vec->len] = i;
	vec->len++;
	return vec->len - 1;
}

matrix* mat_mult(matrix matr1, matrix matr2) {
	return NULL;
}

matrix* strassen_mult(matrix matr1, matrix matr2) {
	return NULL;
}

int main(int argc, char** argv) {
	//flag options: 0 for no extra stuff
	int flag = atoi(argv[1]);
	int dim = atoi(argv[2]);
	char* fname = argv[3];
}
