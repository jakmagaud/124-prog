#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct edge {
	int startpoint, endpoint;
	double weight;
} edge;

typedef struct graph {
	int num_vertices, num_edges;
	edge* edges;
} graph;

//Nodes for disjoint set data structure
typedef struct node {
	int rank;
	struct node* parent;
} node;

//Disjoint set functions
void make_set(node* n);
node* find(node* cur_node);
void link(node* n1, node* n2);
void disj_union(node* n1, node* n2);
