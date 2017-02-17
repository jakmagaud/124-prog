#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Graph edges
typedef struct edge {
	double weight;
} edge;

//Nodes for disjoint set data structure
typedef struct node {
	int rank;
	struct node* parent;
} node;

void make_set(node* n);

node* find(node* cur_node);

void link(node* n1, node* n2);

void disj_union(node* n1, node* n2);
