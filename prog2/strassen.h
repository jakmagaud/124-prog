#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CROSSOVER_PT 90

typedef struct matrix {
	int** data;
	int rows;
	int cols;
} matrix;
