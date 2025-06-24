#ifndef STACK_H
#define STACK_H

#include <stdexcept>


template <typename T>
class Stack {
private:
	struct Node {
		T _data;     
		Node* _next;
		Node(const T& d, Node* n = nullptr) : _data(d), _next(n) {}
	};

	Node* _head; // Указатель на вершину стека

public:
	// Конструктор и Деструктор
	Stack() : _head(nullptr) {}
	~Stack() { clear(); }

	// Добавление элемента в стек
	void push(const T& value) {
		_head = new Node(value, _head);
	}

	// Удаление элемента из стека
	void pop() {
		if (isEmpty()) {
			return;
		}
		Node* temp = _head;
		_head = _head->_next;
		delete temp;
	}

	// Получение верхнего элемента стека
	T& top() {
		if (isEmpty())
			throw std::runtime_error("Ошибка: Попытка получить элемент из пустого стека.");

		return _head->_data;
	}

	// Проверка, пуст ли стек
	bool isEmpty() const {
		return _head == nullptr;
	}

	// Очистка стека
	void clear() {
		while (!isEmpty())
			pop();
	}
};

#endif
