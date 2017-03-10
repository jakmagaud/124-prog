#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "randmst.h"

void make_set(node* n, int i) {
	n->parent = i;
	n->rank = 0;
}

int find(node* sets, int target) {
	if (sets[target].parent != target) {
		sets[target].parent = find(sets, sets[target].parent);
	}
	return sets[target].parent;
}

void link(node* sets, int root1, int root2) {
	if (sets[root1].rank > sets[root2].rank) {
		sets[root2].parent = root1;
	}
	else {
		sets[root1].parent = root2;
		if (sets[root1].rank == sets[root2].rank)
			sets[root2].rank++;
	}
}

void disj_union(node* sets, int root1, int root2) {
	link(sets, find(sets, root1), find(sets, root2));
}
