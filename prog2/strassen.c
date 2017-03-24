#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "strassen.h"

int crossover_pt;

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

void print_mat(matrix* mat) {
	for (int i = 0; i < mat->rows; i++) {
		for (int j = 0; j < mat->cols; j++) {
			printf("%d ", mat->data[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void print_mat_diag(matrix* mat) {
	for (int i = 0, j = 0; i < mat->rows; i++, j++) {
		printf("%d\n", mat->data[i][j]);
	} 
}

matrix* mat_add(matrix* mat1, matrix* mat2, bool sub) {
	assert(mat1->rows == mat2->rows && mat1->cols == mat2->cols);
	matrix* result = mat_init(mat1->rows, mat1->cols);

	for (int i = 0; i < mat1->rows; i++) {
		for (int j = 0; j < mat1->cols; j++) {
			if (!sub)
				result->data[i][j] = mat1->data[i][j] + mat2->data[i][j];
			else
				result->data[i][j] = mat1->data[i][j] - mat2->data[i][j];
		}
	}
	return result;
}

matrix* mat_add_inplace(matrix* mat1, matrix* mat2, bool sub) {
	assert(mat1->rows == mat2->rows && mat1->cols == mat2->cols);

	for (int i = 0; i < mat1->rows; i++) {
		for (int j = 0; j < mat1->cols; j++) {
			if (!sub)
				mat1->data[i][j] = mat1->data[i][j] + mat2->data[i][j];
			else
				mat1->data[i][j] = mat1->data[i][j] - mat2->data[i][j];
		}
	}
	return mat1;
}

void mat_pad(matrix* mat) {
	if (mat->rows % 2 == 0 && mat->rows % 2 == 0)
		return;

	mat->data = realloc(mat->data, (mat->rows + 1) * sizeof(int*));
	mat->data[mat->rows] = calloc(1, mat->cols + 1);

	for (int i = 0; i < mat->rows; i++) {
		mat->data[i] = realloc(mat->data[i], (mat->cols + 1) * sizeof(int));
		mat->data[i][mat->cols] = 0;
	}

	mat->rows++;
	mat->cols++;
}

matrix* mat_mult(matrix* mat1, matrix* mat2) {
	assert(mat1->cols == mat2->rows);
	matrix* result = mat_init(mat1->rows, mat2->cols);
	for (int i = 0; i < mat1->rows; i++) {
		for (int j = 0; j < mat2->cols; j++) {
			for (int k = 0; k < mat1->cols; k++) {
				result->data[i][j] += mat1->data[i][k] * mat2->data[k][j];
			}
		}
	}
	return result;
}

matrix* strassen_mult(matrix* mat1, matrix* mat2) {
	if (mat1->rows/2 <= crossover_pt) 
		return mat_mult(mat1, mat2);

	bool padQ = 0;

	if (mat1->rows % 2 == 1){
		mat_pad(mat1);
		mat_pad(mat2);
		padQ = 1;
	}

	matrix* A = mat_init((mat1->rows)/2, (mat1->cols)/2);
	matrix* B = mat_init((mat1->rows)/2, (mat1->cols)/2);
	matrix* C = mat_init((mat1->rows)/2, (mat1->cols)/2);
	matrix* D = mat_init((mat1->rows)/2, (mat1->cols)/2);
	matrix* E = mat_init((mat2->rows)/2, (mat2->cols)/2);
	matrix* F = mat_init((mat2->rows)/2, (mat2->cols)/2);
	matrix* G = mat_init((mat2->rows)/2, (mat2->cols)/2);
	matrix* H = mat_init((mat2->rows)/2, (mat2->cols)/2);
	
	for (int i = 0; i < mat1->rows/2; i++) {
		memcpy(A->data[i], mat1->data[i], sizeof(int) * mat1->cols/2);
		memcpy(B->data[i], &mat1->data[i][mat1->cols/2], sizeof(int) * mat1->cols/2);
		memcpy(E->data[i], mat2->data[i], sizeof(int) * mat2->cols/2);
		memcpy(F->data[i], &mat2->data[i][mat2->cols/2], sizeof(int) * mat2->cols/2);
	}

	for (int i = mat1->rows/2; i < mat1->rows; i++) {
		memcpy(C->data[i - mat1->rows/2], mat1->data[i], sizeof(int) * mat1->cols/2);
		memcpy(D->data[i - mat1->rows/2], &mat1->data[i][mat1->cols/2], sizeof(int) * mat1->cols/2);
		memcpy(G->data[i - mat1->rows/2], mat2->data[i], sizeof(int) * mat2->cols/2);
		memcpy(H->data[i - mat1->rows/2], &mat2->data[i][mat2->cols/2], sizeof(int) * mat2->cols/2);
	}

	matrix* P1 = strassen_mult(A, mat_add(F, H, 1));
	matrix* P2 = strassen_mult(mat_add(A, B, 0), H);
	matrix* P3 = strassen_mult(mat_add(C, D, 0), E);
	matrix* P4 = strassen_mult(D, mat_add(G, E, 1));
	matrix* P5 = strassen_mult(mat_add(A, D, 0), mat_add(E, H, 0));
	matrix* P6 = strassen_mult(mat_add(B, D, 1), mat_add(G, H, 0));
	matrix* P7 = strassen_mult(mat_add(A, C, 1), mat_add(E, F, 0));

	matrix* AEBG = mat_init((mat2 -> rows)/2, (mat2->cols)/2);
	matrix* AFBH = mat_init((mat2 -> rows)/2, (mat2->cols)/2);
	matrix* CEDG = mat_init((mat2 -> rows)/2, (mat2->cols)/2);
	matrix* CFDH = mat_init((mat2 -> rows)/2, (mat2->cols)/2);

	AEBG = mat_add(mat_add(P5, P4, 0), mat_add(P2, P6, 1), 1); //P5+P4-(P2-P6)
	AFBH = mat_add(P1, P2, 0);
	CEDG = mat_add(P3, P4, 0);
	CFDH = mat_add(mat_add(P5, P1, 0), mat_add(P3, P7, 0), 1); //P5+P1-P3-P7

	matrix* product = mat_init(mat2->rows, mat2->cols);

	for (int i = 0; i < product->rows/2; i++) {
		//product->data[i] = AEBG->data[i];
		memcpy(product->data[i], AEBG->data[i], sizeof(int) * AEBG->cols);
		memcpy(&product->data[i][product->rows/2], AFBH->data[i], sizeof(int) * AFBH->cols);
	}
	for (int j = product->rows/2; j < product->rows; j++) {
		//product->data[j] = CEDG->data[j - product->rows/2];
		memcpy(product->data[j], CEDG->data[j - product->rows/2], sizeof(int) * CEDG->cols);
		memcpy(&product->data[j][product->rows/2], CFDH->data[j - product->rows/2], sizeof(int) * CFDH->cols);
	}

	if (padQ){
		mat1->rows--;
		mat1->cols--;
		mat2->rows--;
		mat2->cols--;
		product->rows--;
		product->cols--;
	}

	return product;
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

int main(int argc, char** argv) {
	//flag options: 0 for no extra stuff
	crossover_pt = atoi(argv[1]);
	int dim = atoi(argv[2]);
	char* fname = argv[3];
	struct timeval t0;
    struct timeval t1;
	matrix* mat1;
	matrix* mat2;

	read_file_mat(fname, dim, &mat1, &mat2);

	printf("Crossover point is %d\n", crossover_pt);
	gettimeofday(&t0, 0);
	matrix* result = mat_mult(mat1, mat2);
	gettimeofday(&t1, 0);
	long elapsed = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec;
	printf("Regular matrix multiplication took %ld microseconds \n", elapsed);
	//print_mat(result);

	gettimeofday(&t0, 0);
	matrix* result2 = strassen_mult(mat1, mat2);
	gettimeofday(&t1, 0);
 	elapsed = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec;
 	printf("Strassen matrix multiplication took %ld microseconds \n", elapsed);
 	printf("\n");
	//print_mat(result2);
}
