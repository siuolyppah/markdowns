#include <iostream>
#include "AvlTree.h"
using namespace std;

// Test program
int main()
{
    AvlTree<int> t;
    
    t.insert(5);
    t.insert(4);
    t.insert(3);
    t.insert(2);
    t.insert(1);


    return 0;
}