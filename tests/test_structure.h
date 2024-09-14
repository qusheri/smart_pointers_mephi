#include <iostream>
#include "../src/smart_ptrs.h"

template<typename T>
struct Node {
    T data;
    UnqPtr<Node<T>> next;

    explicit Node(T val) : data(val), next(nullptr) {}
};

template<typename T>
class LinkedList {
private:
    UnqPtr<Node<T>> head;
    size_t length;

public:
    LinkedList() : head(nullptr), length(0) {}

    void push_front(const T& value) {
        UnqPtr<Node<T>> newNode = UnqPtr<Node<T>>(new Node<T>(value));
        newNode->next = std::move(head);
        head = std::move(newNode);
        ++length;
    }

    bool null(){
        return head.null();
    }

    void pop_front() {
        if (!head.null()) {
            UnqPtr<Node<T>> oldHead = std::move(head);
            head = std::move(oldHead->next);
            --length;
        }
    }

    size_t size() const {
        return length;
    }

    void clear() {
        while (!head.null()) {
            pop_front();
        }
    }
    ~LinkedList(){
        clear();
    }
};
