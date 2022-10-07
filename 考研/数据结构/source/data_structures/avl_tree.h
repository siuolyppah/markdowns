#ifndef MY_AVL_TREE
#define MY_AVL_TREE

#include<algorithm>
#include "dsexceptions.h"

template <typename T>
struct avl_node
{
	using node_ptr = avl_node<T>*;

	T data;
	node_ptr left;
	node_ptr right;
	int h;

	avl_node(T _data, node_ptr left, node_ptr right, int _h = 0)
		:data{_data},left{nullptr}, right(nullptr),h(_h){}
};

template<typename T>
class avl_tree {
public:
	using node_ptr = avl_node<T>*;

private:
	node_ptr root;
	static constexpr int ALLOWED_IMBALANCE = 1;

public:
	avl_tree():root{nullptr}{}

	~avl_tree() {
		make_empty(root);
	}

	void insert(T data) {
		insert(root, data);
	}

	int height(node_ptr t) const;

	void print() const {
		print(root);
	}

	void remove(T data) {
		remove(root, data);
	}

	T find_min() {
		node_ptr t = find_min(root);
		if (t == nullptr) {
			throw UnderflowException{};
		}

		return t->data;
	}

private:
	void make_empty(node_ptr&);

	void insert(node_ptr& t, T data);
	
	void balance(node_ptr& t);

	void rotate_ll(node_ptr& t);

	void rotate_rr(node_ptr& t);

	void rotate_lr(node_ptr& t){
		rotate_rr(t->left);		// 对t的右节点，做左上旋
		rotate_ll(t);
	}

	void rotate_rl(node_ptr& t) {
		rotate_ll(t->right);
		rotate_rr(t);
	}

	void remove(node_ptr& t, T data);

	void print(node_ptr t)const {
		if (t != nullptr) {
			std::cout << t->data << std::endl;

			print(t->left);
			print(t->right);
		}
	}

	node_ptr find_min(node_ptr t) const {
		if (t == nullptr) {
			return nullptr;
		}

		if (t->left == nullptr) {
			return t;
		}

		return find_min(t->left);
	}
};


template<typename T>
inline void avl_tree<T>::make_empty(node_ptr& t)
{
	if (t != nullptr) {
		make_empty(t->left);
		make_empty(t->right);

		std::cout << "destruct:" << t->data << std::endl;
		delete t;
		t = nullptr;
	}
}

template<typename T>
inline void avl_tree<T>::insert(node_ptr& t, T data)
{
	if (t == nullptr) {
		t = new avl_node<T>{ data,nullptr,nullptr };
	}
	
	if (data < t->data) {
		insert(t->left, data);
	}
	else if (t->data < data) {
		insert(t->right, data);
	}

	// 显然，balance是递归向上进行的，即自下而上调整
	balance(t);
}

template<typename T>
inline void avl_tree<T>::balance(node_ptr& t)
{

#define prompt(x) std::cout << x << std::endl;

	if (t == nullptr) {
		return;
	}

	if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) {
		if (height(t->left->left) >= height(t->left->right)) {
			// LL
			rotate_ll(t); prompt("LL");
		}
		else
		{
			// LR
			rotate_lr(t); prompt("LR");
		}

	}
	else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) {
		if (height(t->right->right) >= height(t->left->left)) {
			// RR
			rotate_rr(t); prompt("RR");
		}
		else
		{
			// RL
			rotate_rl(t); prompt("RL");
		}
	}

	t->h = std::max(height(t->left),height(t->right)) + 1;
}

template<typename T>
inline int avl_tree<T>::height(node_ptr t) const
{
	return t == nullptr ? -1 : t->h;
}

template<typename T>
inline void avl_tree<T>::rotate_ll(node_ptr& t)
{
	node_ptr lchild = t->left;
	t->left = lchild->right;
	lchild->right = t;
	t = lchild;
	
	t->h = std::max(height(t->left), height(t->right)) + 1;
	lchild->h = std::max(height(lchild->left), height(lchild->right)) + 1;
}

template<typename T>
inline void avl_tree<T>::rotate_rr(node_ptr& t)
{
	node_ptr rchild = t->right;
	t->right = rchild->left;
	rchild->left = t;
	t = rchild;

	t->h = std::max(height(t->left), height(t->right)) + 1;
	rchild->h = std::max(height(rchild->left), height(rchild->right)) + 1;
}

template<typename T>
inline void avl_tree<T>::remove(node_ptr& t, T data)
{
	if (t == nullptr) {
		return; // Not found, do nothing
	}

	if (data < t->data) {
		remove(t->left, data);
	}
	else if (t->data < data) {
		remove(t->right, data);
	}
	else if (t->left != nullptr && t->right != nullptr) {	// Two children
		// cause two children, find_min() will return a non-null ptr
		t->data =find_min(t->right)->data;
		remove(t->right, t->data);
	}
	else {
		// left and right have at least one nullptr
		node_ptr old = t;
		t = (t->left != nullptr) ? t->left : t->right;
		delete old;
	}

	balance(t);
}


#endif // !MY_AVL_TREE

