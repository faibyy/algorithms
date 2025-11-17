#ifndef H_LINKEDLIST
#define H_LINKEDLIST

#include <cmath>
#include <random>
#include <iostream>
#include <stdexcept>
#include <utility>

template<typename T>
class LinkedList {
public:
	struct Node {
		T data;
		Node* prev;
		Node* next;

		Node(T val, Node* p = nullptr, Node* n = nullptr)
			: data(val), prev(p), next(n) {
		}
	};
private:
	Node* head;
	size_t size_;

	Node* get_node(size_t index) const {
		if (index >= size_) {
			throw std::out_of_range("Index out of bounds");
		}

		Node* current;
		if (index < size_ / 2) {
			current = head;
			for (size_t i = 0; i < index; ++i) {
				current = current->next;
			}
		}
		else {
			current = head->prev;
			for (size_t i = 0; i < (size_ - 1 - index); ++i) {
				current = current->prev;
			}
		}
		return current;
	}
	void clear() {
		while (!is_empty()) {
			pop_head();
		}
	}

public:

	size_t get_size() const { return size_; }
	bool is_empty() const { return size_ == 0; }
	Node* get_head_node() { return head; }
	const Node* get_head_node() const { return head; }
	void set_head_node(Node* newHead) { head = newHead; }

	LinkedList() : head(nullptr), size_(0) {}

	LinkedList(const LinkedList& other) : head(nullptr), size_(0) {
		if (other.is_empty()) {
			return;
		}
		Node* current = other.head;
		do {
			push_tail(current->data);
			current = current->next;
		} while (current != other.head);
	}

	LinkedList(size_t count, int seed = 0) : head(nullptr), size_(0) {
		std::mt19937 gen;
		if (seed == 0) { 
			std::random_device rd; 
			gen.seed(rd());
		}
		else { gen.seed(seed);}
		if constexpr (std::is_same_v<T, bool>) {
			std::uniform_int_distribution<> random(0, 1);
			for (size_t i = 0; i < count; ++i) {
				T value = random(gen);
				push_tail(value);
			}
		}
		else if constexpr (std::is_same_v<T, float>) {
			std::uniform_real_distribution<T> random(0.0, 1.0);
			for (size_t i = 0; i < count; ++i) {
				T value = random(gen);
				push_tail(value);
			}
		}
		else if constexpr (std::is_integral_v<T>)
		{
			std::uniform_int_distribution<T> random(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
			for (size_t i = 0; i < count; ++i) {
				T value = random(gen);
				push_tail(value);
			}
		}
	}

	~LinkedList() {
		clear();
	}

	LinkedList& operator=(const LinkedList& other) {
		if (this == &other) {
			return *this;
		}

		clear();

		if (other.is_empty()) {
			return *this;
		}
		Node* current = other.head;
		do {
			push_tail(current->data);
			current = current->next;
		} while (current != other.head);

		return *this;
	}

	void push_tail(T value) {
		Node* newNode = new Node(value);
		if (!head) {
			head = newNode;
			head->next = head;
			head->prev = head;
		}
		else {
			Node* tail = head->prev;
			tail->next = newNode;
			newNode->prev = tail;
			newNode->next = head;
			head->prev = newNode;
		}
		size_++;
	}

	void push_tail(const LinkedList& other) {
		if (other.is_empty()) {
			return;
		}
		Node* current = other.head;
		do {
			push_tail(current->data);
			current = current->next;
		} while (current != other.head);
	}

	void push_head(T value) {
		push_tail(value);
		head = head->prev;
	}

	void push_head(const LinkedList& other) {
		if (other.is_empty()) {
			return;
		}
		Node* current = other.head->prev;
		do {
			push_head(current->data);
			current = current->prev;
		} while (current != other.head->prev);
	}

	T pop_head() {
		if (!head) {
			throw std::out_of_range("List is empty");
		}

		Node* oldHead = head;
		T value = oldHead->data;

		if (size_ == 1) {
			delete head;
			head = nullptr;
		}
		else {
			Node* tail = head->prev;
			Node* newHead = head->next;
			tail->next = newHead;
			newHead->prev = tail;
			head = newHead;
			delete oldHead;
		}
		size_--;
		return value;
	}

	T pop_tail() {
		if (!head) {
			throw std::out_of_range("List is empty");
		}

		Node* tail = head->prev;
		T value = tail->data;

		if (size_ == 1) {
			delete head;
			head = nullptr;
		}
		else {
			Node* newTail = tail->prev;
			newTail->next = head;
			head->prev = newTail;
			delete tail;
		}
		size_--;
		return value;
	}

	T& operator[](size_t index) {
		if (index >= size_) {
			throw std::out_of_range("Index out of bounds");
		}
		return get_node(index)->data;
	}

	const T& operator[](size_t index) const {
		if (index >= size_) {
			throw std::out_of_range("Index out of bounds");
		}
		return get_node(index)->data;
	}

	friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list) {
		if (list.is_empty()) {
			os << "[]";
			return os;
		}
		os << "[";
		Node* current = list.head;
		do {
			os << current->data;
			current = current->next;
			if (current != list.head) {
				os << ' ';
			}
		} while (current != list.head);
		os << "]";
		return os;
	}
};

template <typename T>
void reverse_list(LinkedList<T>& list) {
	if (list.is_empty() || list.get_size() == 1) {
		return;
	}
	typename LinkedList<T>::Node* current = list.get_head_node();
	typename LinkedList<T>::Node* original_head = current;
	typename LinkedList<T>::Node* new_head = original_head->prev;
	do {
		std::swap(current->prev, current->next);
		current = current->next;
	} while (current != original_head);
	list.set_head_node(new_head);
}

#endif