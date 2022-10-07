#include<iostream>
#include "avl_tree.h"

int main() {
	avl_tree<int> t;

	t.insert(6);
	t.insert(4);
	t.insert(5);
	t.insert(0);

	t.print();

	t.remove(4);
}