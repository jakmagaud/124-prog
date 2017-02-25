#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct edge {
	int startpoint, endpoint;
	double weight;
} edge;

typedef struct vector {
    edge* buf;
    int len;
    int max_len;
} vector;

typedef struct vertex {
	double* coords;
} vertex;

typedef struct graph {
	int num_vertices, num_edges;
	edge* edges;
	vertex* vertices;
} graph;

//Nodes for disjoint set data structure
typedef struct node {
	int rank;
	int parent;
} node;

//Disjoint set functions
void make_set(node* n, int i);
int find(node* sets, int target);
void link(node* sets, int root1, int root2);
void disj_union(node* sets, int root1, int root2);
