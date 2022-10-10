#ifndef MY_BINARY_HEAP
#define MY_BINARY_HEAP

#include<vector>
#include<ostream>

template<typename T>
class binary_heap {
	template<typename T>
	friend void print_binary_heap(const binary_heap<T>& hp, std::ostream& os);

public:
	using size_type = typename std::vector<T>::size_type;

private:
	size_type current_size;
	std::vector<T> arr;

public:

	binary_heap(size_type size)
		:current_size{ 0 }, arr( size * 2 )
	{}

	binary_heap(const std::vector<T> elems)
		:current_size{ elems.size() }, arr( elems.size() * 2 )
	{
		for (decltype(elems.size()) i = 0; i < elems.size(); ++i) {
			arr[i + 1] = elems[i];
		}

		build_tree();
	}

	void insert(T data);

	T remove_min();

	void print() {
		print_binary_heap(*this);
	}

private:
	void build_tree();

	void up(size_type hole);

	void down(size_type hole);
};

template<typename T>
inline void binary_heap<T>::build_tree()
{
	for (size_type i = current_size / 2; i >= 1; --i) {
		down(i);
	}
}

template<typename T>
inline void binary_heap<T>::up(size_type hole)
{
	T tmp = std::move(arr[hole]);

	while (hole > 1 && arr[hole / 2] < arr[hole]) {
		arr[hole] = arr[hole / 2];
		hole /= 2;
	}

	arr[hole] = std::move(tmp);
}

template<typename T>
inline void binary_heap<T>::down(size_type hole)
{
	T tmp = std::move(arr[hole]);

	while (2 * hole <= current_size) {
		size_type x = 2 * hole;
		if (x + 1 <= current_size && arr[x + 1] < arr[x])
			++x;

		if (arr[hole] < arr[x])
			break;

		arr[hole] = arr[x];
		hole = x;
	}

	arr[hole] = std::move(tmp);
}


template<typename T>
inline void binary_heap<T>::insert(T data)
{
	arr[++current_size] = data;

	up(current_size);
}

template<typename T>
inline T binary_heap<T>::remove_min()
{
	T tmp = std::move(arr[1]);

	arr[1] = arr[current_size--];
	down(1);

	return std::move(tmp);
}

#endif // !MY_BINARY_HEAP


