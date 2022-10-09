#include<iostream>
#include "avl_tree.h"

int main() {
	avl_tree<int> t;

	for (int i = 1; i <= 20; ++i) {
		t.insert(i);
	}

	t.sequence_traversal();
}