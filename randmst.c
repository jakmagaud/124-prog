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
	g->vertices = NULL;
	return g;
}

void set_coords(graph* g, int dim) {
	g->vertices = malloc(sizeof(vertex) * g->num_vertices);
	for (int i = 0; i < g->num_vertices; i++) {
		g->vertices[i].coords = malloc(sizeof(double) * dim);
		for (int j = 0; j < dim; j++) {
			g->vertices[i].coords[j] = rng();
		}
	}
}

double distance(vertex v1, vertex v2, int dim) {
	double result = 0;
	switch (dim) {
		case 2: {
			result = sqrt( pow((v1.coords[0] - v2.coords[0]), 2) + pow((v1.coords[1] - v2.coords[1]), 2) );
			break;
		}
		case 3: {
			result = sqrt( pow((v1.coords[0] - v2.coords[0]), 2) + pow((v1.coords[1] - v2.coords[1]), 2) + pow((v1.coords[2] - v2.coords[2]), 2) );
			break;
		}
		case 4: {
			result = sqrt( pow((v1.coords[0] - v2.coords[0]), 2) + pow((v1.coords[1] - v2.coords[1]), 2) + pow((v1.coords[2] - v2.coords[2]), 2) + pow((v1.coords[3] - v2.coords[3]), 2) );
			break;
		}
	}
	return result;
}

void calc_weights(graph* g, int dim) {
	int cur_edge = 0;
	for (int i = 0; i < g->num_vertices; i++) {
		for (int j = i + 1; j < g->num_vertices; j++) {
			g->edges[cur_edge].startpoint = i;
			g->edges[cur_edge].endpoint = j;
			g->edges[cur_edge].weight = distance(g->vertices[i], g->vertices[j], dim);
			cur_edge++;
		}
	}
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
			set_coords(g, 2);
			calc_weights(g, 2);
			break;
		}
		case 3: {
			set_coords(g, 3);
			calc_weights(g, 3);
			break;
		}
		case 4: {
			set_coords(g, 4);
			calc_weights(g, 4);
			break;
		}
	}
	return g;
}

int edge_comp(const void* a, const void* b) {
	edge* e1 = (edge*) a;
	edge* e2 = (edge*) b;
	return e1->weight > e2->weight;
}

edge* kruskal(graph* g) {
	//sort edges according to weight in ascending order
	qsort(g->edges, g->num_edges, sizeof(edge), edge_comp);
	edge* MST_edges = malloc(sizeof(edge) * (g->num_vertices - 1));
	node* sets = malloc(sizeof(node) * g->num_vertices);
	int curr_edge_index = 0;

	for (int i = 0; i < g->num_vertices; i++){
		sets[i] = make_set(g->vertices[i]);
	}

	for (int i = 0; i < g->num_edges; i++){
		if find(g->edges[i].startpoint) != find(g->edges[i].endpoint){
			MST_edges[curr_edge_index] = g->edges[i];
			curr_edge_index += 1;
			disj_union(sets[g->edges[i].startpoint], sets[g->edges[i].endpoint]);
		}
	}
	return MST_edges;
}



void set_coords(graph* g, int dim) {
	g->vertices = malloc(sizeof(vertex) * g->num_vertices);
	for (int i = 0; i < g->num_vertices; i++) {
		g->vertices[i].coords = malloc(sizeof(double) * dim);
		for (int j = 0; j < dim; j++) {
			g->vertices[i].coords[j] = rng();
		}
	}
}


int main(int argc, char** argv) {
	int flag = atoi(argv[1]);
	int num_pts = atoi(argv[2]);
	int num_trials = atoi(argv[3]);
	int dim = atoi(argv[4]);

	graph* g = graph_generator(num_pts, num_trials, dim);

	/* For timing, when we get there
	struct timeval t0;
    struct timeval t1;

    gettimeofday(&t0, 0);
    gettimeofday(&t1, 0);
    long elapsed = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec;
    printf("Operation took %ld microseconds\n", elapsed);
    */

	//cleanup
	free(g->edges);
	if (g->vertices) {
		for (int i = 0; i < g->num_vertices; i++) {
			free(g->vertices[i].coords);
		}
		free(g->vertices);
	}
	free(g);
}
