#include <iostream>

template <typename T>
struct binary_node
{
    T data;
    binary_node *left;
    binary_node *right;

    binary_node(T _data)
        : data(_data), left(nullptr), right(nullptr)
    {
    }
};

template <typename T>
class binary_tree
{
public:
    binary_node<T> *root;

    binary_tree(/* args */);
    ~binary_tree();
};

template <typename T>
binary_tree<T>::binary_tree(/* args */)
    : root(nullptr)
{
}

template <typename T>
binary_tree<T>::~binary_tree()
{
    preorder_traversal(root, [](binary_node<T> *node_ptr) -> void
                       {
                           std::cout << "destruct:" << node_ptr->data << std::endl;
                           delete node_ptr; });
}

/**
 * @brief 先序遍历某棵树
 *
 * @tparam T 元素的类型
 * @tparam R 对当前节点进行的操作的返回类型
 * @param root 某棵树的根节点
 * @param p 对当前节点进行的操作
 */
template <typename T, typename F>
void preorder_traversal(binary_node<T> *root, F fun)
{
    if (root != nullptr)
    {
        auto left = root->left;
        auto right = root->right;
        fun(root);

        preorder_traversal(left, fun);
        preorder_traversal(right, fun);
    }
}

template <typename T>
bool preorder_traversal(binary_node<T> *root, bool (*p)(binary_node<T> *))
{
    if (root != nullptr)
    {
        auto left = root->left;
        auto right = root->right;
        bool b = p(root);

        return b || preorder_traversal(left, p) || preorder_traversal(right, p);
    }

    return false;
}

template <typename T>
void print_tree(const binary_tree<T> &tree)
{
    preorder_traversal(tree.root, [](binary_node<T> *node_ptr)
                       { std::cout << node_ptr->data << std::endl; });
}

template <typename T>
bool contains(const binary_tree<T> &tree, T key)
{
    return preorder_traversal(tree.root, [&key](binary_node<T> *node_ptr)
                              { return node_ptr->data == key; });
}

template <typename T>
void insert(binary_node<T> *&root, T key)
{
    if (root == nullptr)
    {
        root = new binary_node<T>(key);
    }
    else
    {
        if (key < root->data)
        {
            insert(root->left, key);
        }
        else if (root->data < key)
        {
            insert(root->right, key);
        }
    }
}

int main()
{

    binary_tree<int> tree;
    insert(tree.root, 6);
    insert(tree.root, 2);
    insert(tree.root, 1);
    insert(tree.root, 5);
    insert(tree.root, 3);
    insert(tree.root, 4);
    insert(tree.root, 8);

    return 0;
}