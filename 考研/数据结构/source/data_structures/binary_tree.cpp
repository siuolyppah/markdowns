#include <iostream>
#include "binary_tree.h"

int main() {
	binary_tree<int> t;

    t.insert(6);
    t.insert(2);
    t.insert(1);
    t.insert(5);
    t.insert(3);
    t.insert(4);
    t.insert(8);

    t.print_tree();
    std::cout << std::endl;
    t.remove(2);
    t.print_tree();
    std::cout << std::endl;

	return 0;
}

