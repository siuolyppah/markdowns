#include <iostream>

template <typename T>
struct Node
{
    T data;
    Node<T> *prev;
    Node<T> *next;

    Node() : prev(nullptr), next(nullptr) {}
    Node(T elem) : data(elem), prev(nullptr), next(nullptr) {}
};

template <typename T>
class LinkedList
{
private:
    Node<T> *head;
    Node<T> *tail;

public:
    LinkedList();

    void insert_head(T elem);

    void insert_tail(T elem);

    void print_list();

    void clear();

    ~LinkedList();
};

template <typename T>
LinkedList<T>::LinkedList()
    : head(new Node<T>()), tail(new Node<T>())
{
    head->next = tail;
    tail->prev = head;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
    Node<T> *current_node = head;
    Node<T> *next_node = head->next;

    while (current_node != nullptr)
    {
        std::cout << "current elem:" << current_node->data << std::endl;
        delete current_node;
        current_node = next_node;
        next_node = next_node->next;
    };
}

template <typename T>
void LinkedList<T>::insert_head(T elem)
{
    Node<T> *temp = new Node<T>(elem);
    Node<T> *behind_head = head->next;

    head->next = temp;
    temp->prev = head;

    temp->next = behind_head;
    behind_head->prev = temp;
}

template <typename T>
void LinkedList<T>::insert_tail(T elem)
{
    Node<T> *temp = new Node<T>(elem);
    Node<T> *before_tail = tail->prev;

    before_tail->next = temp;
    temp->prev = before_tail;

    temp->next = tail;
    tail->prev = temp;
}

template <typename T>
void LinkedList<T>::print_list()
{
    Node<T> *temp = head->next;
    while (temp != tail)
    {
        std::cout << temp->data << std::endl;
        temp = temp->next;
    }
}

template <typename T>
void LinkedList<T>::clear()
{
    Node<T> *current_node = head->next;
    Node<T> *next_node = head->next->next;

    while (current_node != tail)
    {
        std::cout << "current elem:" << current_node->data << std::endl;
        delete current_node;
        current_node = next_node;
        next_node = next_node->next;
    };

    head->next = tail;
    tail->prev = head;
}

int main()
{
    LinkedList<int> list;
    list.insert_tail(1);
    list.insert_tail(2);
    list.insert_tail(3);

    list.print_list();

    list.clear();
    std::cout << std::endl;

    list.insert_tail(1);
    list.insert_tail(2);
}
