#include<iostream>
#include<vector>
#include"binary_heap.h"


template<typename T>
void print_binary_heap(const binary_heap<T>& hp, std::ostream& os = std::cout)
{
	unsigned long divide = 2;

	typename binary_heap<T>::size_type  pos = 1;
	while (pos <= hp.current_size) {
		os << hp.arr[pos] << ' ';

		if ((pos + 1) % divide == 0) {
			divide <<= 1;
			os << std::endl;
		}

		++pos;
	}

}



int main() {

	std::vector<int> v{ 13,21,16,24,31,19,68,65,26,32 };
	binary_heap<int> hp{ v };


	hp.insert(14);

	hp.print();

	return 0;
}


