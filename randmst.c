#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>

typedef struct edge {
	double weight;
	int** coords;
} edge;

typedef struct node {

} node;

int main(int argc, char** argv) {
	int flag = atoi(argv[1]);
	int num_points = atoi(argv[2]);
	int num_trials = atoi(argv[3]);
	int dim = atoi(argv[4]);

}
