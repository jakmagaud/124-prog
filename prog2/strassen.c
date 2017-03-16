#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "strassen.h"

matrix* mat_init(int rows, int cols) {
	matrix* mat = malloc(sizeof(matrix));
	mat->rows = rows;
	mat->cols = cols;
	mat->data = calloc(1, sizeof(int) * rows * cols);
	return mat;
}

int mat_index(matrix mat, int r, int c) {
	int index = r * mat.cols + c;
	assert(index < mat.rows * mat.cols);
	return index;
}

matrix* mat_add(matrix mat1, matrix mat2, bool sub) {
	assert(mat1.rows == mat2.rows && mat1.cols == mat2.cols);
	matrix* result = mat_init(mat1.rows, mat1.cols);

	for (int i = 0; i < mat1.rows; i++) {
		for (int j = 0; j < mat1.cols; j++) {
			int ind = mat_index(mat1, i, j);
			if (!sub)
				result->data[ind] = mat1.data[ind] + mat2.data[ind];
			else
				result->data[ind] = mat1.data[ind] - mat2.data[ind];
		}
	}
	return result;
}

matrix* mat_pad(matrix mat) {
	return NULL;
}

matrix* mat_mult(matrix mat1, matrix mat2) {
	assert(mat1.cols == mat2.rows);
	matrix* result = mat_init(mat1.rows, mat2.cols);
	for (int i = 0; i < mat1.rows; i++) {
		for (int j = 0; j < mat2.cols; j++) {
			for (int k = 0; k < mat1.cols; k++) {
				int ind1 = mat_index(*result, i, j);
				int ind2 = mat_index(mat1, i, k);
				int ind3 = mat_index(mat2, k, j);
				result->data[ind1] += mat1.data[ind2] * mat2.data[ind3];
			}
		}
	}
	return result;
}

matrix* strassen_mult(matrix mat1, matrix mat2) {
	return NULL;
}

int main(int argc, char** argv) {
	//flag options: 0 for no extra stuff
	// int flag = atoi(argv[1]);
	// int dim = atoi(argv[2]);
	// char* fname = argv[3];

	matrix* mat1 = mat_init(2,2);
	matrix* mat2 = mat_init(2,2);
	mat1->data[0] = 1;
	mat1->data[1] = 2;
	mat1->data[2] = 3;
	mat1->data[3] = 4;
	mat2->data[0] = 4;
	mat2->data[1] = 3;
	mat2->data[2] = 2;
	mat2->data[3] = 1;
	matrix* result = mat_mult(*mat1, *mat2);
}
