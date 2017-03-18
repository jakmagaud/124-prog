#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
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

void read_file_mat(char* fname, int dim, matrix** mat1, matrix** mat2) {
	*mat1 = mat_init(dim, dim);
	*mat2 = mat_init(dim, dim);
	FILE* file = fopen(fname, "r");

	int tmp = 0;
	for (int i = 0; i < pow(dim,2); i++) {
		int r = i / dim;
		int c = i % dim;
		fscanf(file, "%d", &tmp);
		(*mat1)->data[r][c] = tmp;
	}
	for (int j = 0; j < pow(dim,2); j++) {
		int r = j / dim;
		int c = j % dim;
		fscanf(file, "%d", &tmp);
		(*mat2)->data[r][c] = tmp;
	}
	fclose(file);
}

void print_mat(matrix* mat) {
	for (int i = 0; i < mat->rows; i++) {
		for (int j = 0; j < mat->cols; j++) {
			printf("%d ", mat->data[i][j]);
		}
		printf("\n");
	}
}

void print_mat_diag(matrix* mat) {
	for (int i = 0, j = 0; i < mat->rows; i++, j++) {
		printf("%d\n", mat->data[i][j]);
	} 
}

int main(int argc, char** argv) {
	//flag options: 0 for no extra stuff
	int flag = atoi(argv[1]);
	int dim = atoi(argv[2]);
	char* fname = argv[3];

	matrix* mat1;
	matrix* mat2;
	read_file_mat(fname, dim, &mat1, &mat2);
	matrix* result = mat_mult(*mat1, *mat2);
	print_mat(result);
	print_mat_diag(result);

	// struct timeval t0;
    // struct timeval t1;
    // gettimeofday(&t0, 0);
 	// gettimeofday(&t1, 0);
 	// long elapsed = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec;
}
