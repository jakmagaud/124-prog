#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include "randmst.h"

//returns random number between 0 and 1
double rng(void) {
	return (double) rand() / (double) ((unsigned) RAND_MAX + 1);
}

graph* create_graph(int num_vertices, int num_edges) {
	graph* g = malloc(sizeof(graph));
	g->num_vertices = num_vertices;
	g->num_edges = num_edges;
	g->edges = malloc(sizeof(edge) * num_edges);
	return g;
}

graph* graph_generator(int num_pts, int num_trials, int dim) {
	graph* g = create_graph(num_pts, num_pts * (num_pts - 1) / 2);
	switch (dim) {
		case 1: {
			int cur_edge = 0;
			for (int i = 0; i < g->num_vertices; i++) {
				for (int j = i + 1; j < g->num_vertices; j++) {
					g->edges[cur_edge].startpoint = i;
					g->edges[cur_edge].endpoint = j;
					g->edges[cur_edge].weight = rng();
					cur_edge++;
				}
			}
			break;
		}

		case 2: {
			break;
		}

		case 3: {
			break;
		}

		case 4: {
			break;
		}
	}
	return g;
}

void kruskal(graph* g) {

}

int main(int argc, char** argv) {
	int flag = atoi(argv[1]);
	int num_pts = atoi(argv[2]);
	int num_trials = atoi(argv[3]);
	int dim = atoi(argv[4]);

	graph* g = graph_generator(num_pts, num_trials, dim);

	printf("hi");

}
