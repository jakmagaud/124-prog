#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "strassen.h"

matrix* mat_init(int rows, int cols) {
	matrix* mat = malloc(sizeof(matrix));
	mat->rows = rows;
	mat->cols = cols;
	mat->data = malloc(rows * sizeof(int*));
	for (int i = 0; i < rows; i++) {
		mat->data[i] = calloc(1, cols * sizeof(int));
	}
	return mat;
}

matrix* mat_add(matrix mat1, matrix mat2, bool sub) {
	assert(mat1.rows == mat2.rows && mat1.cols == mat2.cols);
	matrix* result = mat_init(mat1.rows, mat1.cols);

	for (int i = 0; i < mat1.rows; i++) {
		for (int j = 0; j < mat1.cols; j++) {
			if (!sub)
				result->data[i][j] = mat1.data[i][j] + mat2.data[i][j];
			else
				result->data[i][j] = mat1.data[i][j] - mat2.data[i][j];
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
				result->data[i][j] += mat1.data[i][k] * mat2.data[k][j];
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
	mat1->data[0][0] = 1;
	mat1->data[0][1] = 2;
	mat1->data[1][0] = 3;
	mat1->data[1][1] = 4;
	mat2->data[0][0] = 4;
	mat2->data[0][1] = 3;
	mat2->data[1][0] = 2;
	mat2->data[1][1] = 1;
	matrix* result = mat_mult(*mat1, *mat2);
}
