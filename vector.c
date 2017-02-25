#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "randmst.h"

#define INIT_VEC_LEN 256

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

vector* concat_vector(vector* vec1, vector* vec2) {
	int new_len = vec1->len + vec2->len;
	int new_max_len = new_len * 2;

	vec1->buf = realloc(vec1->buf, new_max_len * sizeof(edge));
	memcpy(vec1->buf + vec1->len, vec2->buf, vec2->len * sizeof(edge));

	vec1->len = new_len;
	vec1->max_len = new_max_len;

	return vec1;
}

vector* copy_vector(vector* vec) {
	vector* copy = create_vector(vec->len);
	concat_vector(copy, vec);
	return copy;
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
