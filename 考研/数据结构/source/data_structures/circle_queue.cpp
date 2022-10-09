#include<iostream>
#include "cicle_queue.h"

int main() {
	circle_queue<int> cq;

	for (size_t i = 0; i < 7; i++)
	{
		cq.push(i);
	}

	for (size_t i = 0; i < 5; i++) {
		std::cout << cq.begin() << std::endl;
		cq.pop();
	}

	for (size_t i = 0; i < 10; i++)
	{
		cq.push(i);
	}
}
