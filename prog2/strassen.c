#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <string.h>
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
	if (mat1->rows <= CROSSOVER_PT) 
		return mat_mult(mat1, mat2);

	bool padQ = 0;

	if (mat1->rows % 2 == 1){
		mat_pad(mat1);
		mat_pad(mat2);
		padQ = 1;
	}

	matrix* A = mat_init((mat1->rows)/2, (mat1->cols)/2);
	for (int i = 0; i < (mat1->rows)/2; i++){
		for (int j = 0; j < (mat1 -> cols)/2; j++){
			A->data[i][j] = mat1->data[i][j];
		}
	}

	matrix* B = mat_init((mat1->rows)/2, (mat1->cols)/2);
	for (int i = 0; i < (mat1->rows)/2; i++){
		for (int j = (mat1 -> cols)/2; j < mat1 -> cols; j++){
			B->data[i][j - (mat1->cols)/2] = mat1->data[i][j];
		}
	}


	matrix* C = mat_init((mat1->rows)/2, (mat1->cols)/2);
	for (int i = (mat1->rows)/2; i < mat1->rows; i++){
		for (int j = 0; j < (mat1 -> cols)/2; j++){
			C->data[i-(mat1->rows)/2][j] = mat1->data[i][j];
		}
	}

	matrix* D = mat_init((mat1->rows)/2, (mat1->cols)/2);
	for (int i = (mat1->rows)/2; i < mat1->rows; i++){
		for (int j = (mat1->cols)/2; j < mat1 -> cols; j++){
			D->data[i-(mat1->rows)/2][j-(mat1->cols)/2] = mat1->data[i][j];
		}
	}


	matrix* E = mat_init((mat2->rows)/2, (mat2->cols)/2);
	for (int i = 0; i < (mat2->rows)/2; i++){
		for (int j = 0; j < (mat2 -> cols)/2; j++){
			E->data[i][j] = mat2->data[i][j];
		}
	}

	matrix* F = mat_init((mat2->rows)/2, (mat2->cols)/2);
	for (int i = 0; i < (mat2->rows)/2; i++){
		for (int j = (mat2 -> cols)/2; j < mat2 -> cols; j++){
			F->data[i][j-(mat2 -> cols)/2] = mat2->data[i][j];
		}
	}

	matrix* G = mat_init((mat2->rows)/2, (mat2->cols)/2);
	for (int i = (mat2->rows)/2; i < mat2->rows; i++){
		for (int j = 0; j < (mat2 -> cols)/2; j++){
			G->data[i-(mat2 -> rows)/2][j] = mat2->data[i][j];
		}
	}


	matrix* H = mat_init((mat2->rows)/2, (mat2->cols)/2);
	for (int i = (mat2->rows)/2; i < mat2->rows; i++){
		for (int j = (mat2->cols)/2; j < mat2 -> cols; j++){
			H->data[i-(mat2 -> rows)/2][j-(mat2 -> cols)/2] = mat2->data[i][j];
		}
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
		memcpy(product->data[i], AEBG->data[i], sizeof(int) * AEBG->cols);
		memcpy(&product->data[i][product->rows/2], AFBH->data[i], sizeof(int) * AFBH->cols);
	}
	for (int j = product->rows/2; j < product->rows; j++) {
		memcpy(product->data[j], CEDG->data[j - product->rows/2], sizeof(int) * CEDG->cols);
		memcpy(&product->data[j][product->rows/2], CFDH->data[j - product->rows/2], sizeof(int) * CFDH->cols);
	}

	// for (int i = 0; i < mat2->rows; i++){
	// 	for (int j = 0; j < mat2 -> cols; j++){
	// 		if (i < (mat2->rows)/2){
	// 			if (j < (mat2 -> rows)/2){
	// 				product->data[i][j] = AEBG->data[i][j];
	// 			}
	// 			else{
	// 				product->data[i][j] = AFBH->data[i][j-(mat2->cols)/2];
	// 			}
	// 		}
	// 		else{
	// 			if (j < (mat2 -> rows)/2){
	// 				product-> data[i][j] = CEDG->data[i-(mat2 -> rows)/2][j];
	// 			}
	// 			else{
	// 				product -> data[i][j] = CFDH->data[i - (mat2->rows)/2][j-(mat2->cols)/2];
	// 			}
	// 		}
	// 	}
	// }

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
	int flag = atoi(argv[1]);
	int dim = atoi(argv[2]);
	char* fname = argv[3];
	struct timeval t0;
    struct timeval t1;
	matrix* mat1;
	matrix* mat2;

	read_file_mat(fname, dim, &mat1, &mat2);

	// matrix* A = malloc(sizeof(matrix));
	// A->rows = mat1->rows/2;
	// A->cols = mat1->cols/2;
	// A->data = malloc(A->rows * sizeof(int*));
	// for (int i = 0; i < A->rows; i++) {
	// 	A->data[i] = mat1->data[i];
	// }
	// print_mat(A);
	// matrix* B = malloc(sizeof(matrix));
	// B->rows = mat1->rows/2;
	// B->cols = mat1->cols/2;
	// B->data = malloc(B->rows * sizeof(int*));
	// for (int i = 0; i < B->rows; i++) {
	// 	B->data[i] = &mat1->data[i][mat1->cols/2];
	// }
	// print_mat(B);
	// matrix* C = malloc(sizeof(matrix));
	// C->rows = mat1->rows/2;
	// C->cols = mat1->cols/2;
	// C->data = malloc(C->rows * sizeof(int*));
	// for (int i = 0; i < C->rows; i++) {
	// 	C->data[i] = mat1->data[i] + (sizeof(int) * mat1->rows/2);
	// }
	// print_mat(C);
	// matrix* D = malloc(sizeof(matrix));
	// D->rows = mat1->rows/2;
	// D->cols = mat1->cols/2;
	// D->data = malloc(D->rows * sizeof(int*));
	// for (int i = 0; i < D->rows; i++) {
	// 	D->data[i] = &mat1->data[i][mat1->cols/2] + (sizeof(int) * mat1->rows/2);
	// }
	// print_mat(D);

	gettimeofday(&t0, 0);
	matrix* result = mat_mult(mat1, mat2);
	gettimeofday(&t1, 0);
	long elapsed = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec;
	printf("Regular matrix multiplication took %ld microseconds \n", elapsed);
	print_mat(result);
	gettimeofday(&t0, 0);
	matrix* result2 = strassen_mult(mat1, mat2);
	gettimeofday(&t1, 0);
	print_mat(result2);
 	elapsed = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec;
 	printf("Strassen matrix multiplication took %ld microseconds \n", elapsed);
 	print_mat(result);

}
