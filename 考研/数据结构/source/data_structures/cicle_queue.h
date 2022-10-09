#ifndef MY_CIRCLE_QUEUE
#define MY_CIRCLE_QUEUE

#include <iostream>

template <typename T>
class circle_queue
{
private:
	static constexpr int MAXSIZE = 10;

	T* ptr;

	int front;
	int rear;

public:
	circle_queue() :front{ 0 }, rear{ 0 }, ptr{ new T[MAXSIZE] } {}

	void push(T data);

	void pop();

	T begin();

	bool empty() {
		return front == rear;
	}

	bool full() {
		return (rear + 1) % MAXSIZE == front;
	}

};

#define prompt(str) std::cout << str << std::endl

template<typename T>
inline void circle_queue<T>::push(T data)
{
	if (!full())
	{
		ptr[rear] = data;
		rear = (rear + 1) % MAXSIZE;
	}
	else
		prompt("push error: already full!");
}

template<typename T>
inline void circle_queue<T>::pop()
{
	if (!empty()) {
		front = (front + 1) % MAXSIZE;
	}
	else
		prompt("pop error: it's empty now!");
}

template<typename T>
inline T circle_queue<T>::begin()
{
	if (!empty()) {
		return ptr[front];
	}
	else 
		prompt("pop error: it's empty now!");
}
#endif // !MY_CIRCLE_QUEUE
