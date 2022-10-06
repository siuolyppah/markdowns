#ifndef MY_BINARY_TREE

#include "dsexceptions.h"

template<typename T>
struct binary_node {
	T data;
	binary_node* left;
	binary_node* right;

	binary_node(T _data):data(_data),left(nullptr),right(nullptr) {}
};

template<typename T>
struct binary_tree
{
private:
	binary_node<T>* root;

public:

	binary_tree() : root(nullptr) {}

	~binary_tree() {
		make_empty(root);
	}

	void insert(T elem) {
		insert(root, elem);
	}

	void remove(T elem) {
		remove(root, elem);
	}

	const T& find_min(){
		auto t = find_min(root);

		if (t == nullptr) {
			throw UnderflowException{};
		}

		return t->data;
	}

	void print_tree() {
		print_tree(root);
	}

private:
	void insert(binary_node<T>*& t, T elem) {
		if (t == nullptr) {
			t = new binary_node<T>(elem);
			return;
		}

		if (t->data < elem) {
			insert(t->right, elem);
		}
		else if (elem < t->data) {
			insert(t->left, elem);
		}
		else {
			;	// duplicate,do nothing
		}
	}

	void remove(binary_node<T>*& t, T elem) {
		if (t == nullptr) {
			;	// not exists,do nothing
		}

		if (t->data < elem) {
			remove(t->right, elem);
		}
		else if (elem < t->data) {
			remove(t->left, elem);
		}
		else if (t->left!=nullptr && t->right != nullptr) {
			T min =find_min(t->right)->data;
			t->data = min;

			remove(t->right, min);
		}
		else {
			binary_node<T>* old = t;
			t = (t->left != nullptr) ? t->left : t->right;
			delete old;
		}
	}

	binary_node<T>* find_min( binary_node<T>* t) {
		if (t == nullptr) {
			return nullptr;
		}

		if (t->left == nullptr)
		{
			return t;
		}
		
		return find_min(t->left);
	}

	void print_tree(const binary_node<T>* t) {
		if (t == nullptr) {
			return;
		}

		std::cout << t->data << std::endl;
		print_tree(t->left);
		print_tree(t->right);
	}

	void make_empty(binary_node<T>*& t) {
		if (t != nullptr) {
			make_empty(t->left);
			make_empty(t->right);
			std::cout << "make_empty:" << t->data << std::endl;
			delete t;
		}

		t = nullptr;
	}
};


#endif // !MY_BINARY_TREE