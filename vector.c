#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "randmst.h"

vector* create_vector(int len) {
	vector* vec = malloc(sizeof(vector));
	if (len < INIT_VEC_LEN)
		len = INIT_VEC_LEN;
	
	vec->buf = malloc(len * sizeof(edge));
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

int vector_insert(vector* vec, edge e) {
	if (vec->len >= vec->max_len) {
		vec->max_len *= 2;
		vec->buf = realloc(vec->buf, vec->max_len * sizeof(edge));
	}

	vec->buf[vec->len] = e;
	vec->len++;

	return vec->len - 1;
}
