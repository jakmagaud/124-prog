#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "randmst.h"

void make_set(node* n) {
	n->parent = n;
	n->rank = 0;
}

node* find(node* cur_node) {
	if (cur_node != cur_node->parent) {
		cur_node->parent = find(cur_node->parent);
	}

	return cur_node->parent;
}

void link(node* n1, node* n2) {
	if (n1->rank > n2->rank) {
		n2->parent = n1;
	}
	else {
		n1->parent = n2;
		if (n1->rank == n2->rank)
			n2->rank += 1;
	}
}

void disj_union(node* n1, node* n2) {
	link(find(n1), find(n2));
}
