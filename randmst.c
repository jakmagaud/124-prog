#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include "randmst.h"

//returns random number between 0 and 1
double rng(void) {
	return (double) rand() / (double) ((unsigned) RAND_MAX + 1);
}

double edge_weight_bound(int num_vertices, int dim) {
	if (dim == 0) {
		if (num_vertices < 300) 
			return 1.0;
		if (num_vertices > 10000) 
			return 0.00205;
		return (1.0/((double) num_vertices * 0.0493 + 2.13589339));
	}
	else if (dim == 2){
		if (num_vertices < 300)
			return 1.0;
		if (num_vertices > 2000)
			return 0.06777;
		return (1.0/((double) num_vertices * 0.006519+ 1.717));
	}
	return 1.0;
}

graph* create_graph(int num_vertices, int num_edges) {
	graph* g = malloc(sizeof(graph));
	g->num_vertices = num_vertices;
	g->num_edges = num_edges;
	g->edges = create_vector(INIT_VEC_LEN);
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

void calc_weights(graph* g, int dim, double bound) {
	//int cur_edge = 0;

	for (int i = 0; i < g->num_vertices; i++) {
		for (int j = i + 1; j < g->num_vertices; j++) {
			double weight = distance(g->vertices[i], g->vertices[j], dim);
			if (weight > bound) {
				continue;
			}
			edge* e = malloc(sizeof(edge));
			e->startpoint = i;
			e->endpoint = j;
			e->weight = weight;
			vector_insert(g->edges, *e);
			/*
			g->edges[cur_edge].startpoint = i;
			g->edges[cur_edge].endpoint = j;
			g->edges[cur_edge].weight = distance(g->vertices[i], g->vertices[j], dim);
			cur_edge++;
			*/
		}
	}
}

graph* graph_generator(int num_pts, int num_trials, int dim, int flag) {
	graph* g = create_graph(num_pts, num_pts * (num_pts - 1) / 2);

	double bound = edge_weight_bound(num_pts, dim);
	if (flag == 2 || flag == 3) {
		bound = 1.0;
	}

	switch (dim) {
		case 0: {
			//int cur_edge = 0;
			for (int i = 0; i < g->num_vertices; i++) {
				for (int j = i + 1; j < g->num_vertices; j++) {
					double weight = rng();
					if (weight > bound) {
						continue;
					}
					edge* e = malloc(sizeof(edge));
					e->startpoint = i;
					e->endpoint = j;
					e->weight = weight;
					vector_insert(g->edges, *e);
					/*
					g->edges[cur_edge].startpoint = i;
					g->edges[cur_edge].endpoint = j;
					g->edges[cur_edge].weight = rng();
					cur_edge++;
					*/
				}
			}
			break;
		}
		case 2: {
			set_coords(g, 2);
			calc_weights(g, 2, bound);
			break;
		}
		case 3: {
			set_coords(g, 3);
			calc_weights(g, 3, bound);
			break;
		}
		case 4: {
			set_coords(g, 4);
			calc_weights(g, 4, bound);
			break;
		}
	}
	return g;
}

int edge_comp(const void* a, const void* b) {
	edge* e1 = (edge*) a;
	edge* e2 = (edge*) b;
	if (e1->weight > e2->weight) return 1;
	else if (e1->weight < e2->weight) return -1;
	else return 0;
}

edge* kruskal(graph* g, int dim) {
	//sort edges according to weight in ascending order
	qsort(g->edges->buf, g->edges->len, sizeof(edge), edge_comp);

	edge* MST_edges = malloc(sizeof(edge) * (g->num_vertices - 1));
	node* sets = malloc(sizeof(node) * g->num_vertices);

	for (int i = 0; i < g->num_vertices; i++) {
		make_set(&sets[i], i);
	}

	int mst_edge_index = 0;
	for (int i = 0; i < g->edges->len; i++) {
		int startpoint_root = find(sets, g->edges->buf[i].startpoint);
		int endpoint_root = find(sets, g->edges->buf[i].endpoint);
		if (startpoint_root != endpoint_root) {
			MST_edges[mst_edge_index] = g->edges->buf[i];
			mst_edge_index++;
			disj_union(sets, startpoint_root, endpoint_root);
		}
	}

	return MST_edges;
}

int main(int argc, char** argv) {
	//flag options: 0 for no extra stuff, 1 for timing, 2 for no bounds, 3 for no bounds and timing

	int flag = atoi(argv[1]);
	int num_pts = atoi(argv[2]);
	int num_trials = atoi(argv[3]);
	int dim = atoi(argv[4]);

	srand((unsigned) time(NULL));
	struct timeval t0;
    struct timeval t1;

    double avg_sum[num_trials];
    double max_max_edge = 0;

	for (int i = 0; i < num_trials; i++) {
		graph* g = graph_generator(num_pts, num_trials, dim, flag);
		if (flag == 1 || flag == 3)
    		gettimeofday(&t0, 0);
		edge* mst = kruskal(g, dim);

		// printf("%f", mst[num_pts - 2].weight);
		if (flag == 1 || flag == 3) {
    		gettimeofday(&t1, 0);
    		long elapsed = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec;
			printf("Graph number %d took %ld microseconds to find an MST for\n", i + 1, elapsed);
		}

		if (max_max_edge < mst[num_pts - 2].weight) {
			max_max_edge = mst[num_pts - 2].weight;
		}

    	double sum = 0;
    	for (int j = 0; j < num_pts - 1; j++) {
    		sum += mst[j].weight;
    	}
    	avg_sum[i] = sum;
		// printf("The weight of graph number %d of MST is %f\n", i, sum);
    	// printf("max edge in MST is %f\n", mst[num_pts - 2].weight);

		//cleanup
		//free(mst);
		// free_vector(g->edges);
		// if (g->vertices) {
		// 	for (int i = 0; i < g->num_vertices; i++) {
		// 		free(g->vertices[i].coords);
		// 	}
		// 	free(g->vertices);
		// }
		// free(g);
	}
	//printf("%f", edge_weight_bound(num_pts, dim));

	double sum = 0;
	for (int i = 0; i < num_trials; i++) {
		sum += avg_sum[i];
	}
	double overall_avg = sum / (num_trials);
	printf("Average MST weight was %f\n", overall_avg);
	//printf("%f", max_max_edge);
}
