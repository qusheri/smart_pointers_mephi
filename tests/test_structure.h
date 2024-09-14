#include <iostream>
#include "../src/smart_ptrs.h"

template<typename T>
struct NodeUnique {
    T data;
    UnqPtr<NodeUnique<T>> next;

    explicit NodeUnique(T val) : data(val), next(nullptr) {}
};

template<typename T>
class LinkedListUnique {
private:
    UnqPtr<NodeUnique<T>> head;
    size_t length;

public:
    LinkedListUnique() : head(nullptr), length(0) {}

    void push_front(const T& value) {
        UnqPtr<NodeUnique<T>> newNode = UnqPtr<NodeUnique<T>>(new NodeUnique<T>(value));
        newNode->next = std::move(head);
        head = std::move(newNode);
        ++length;
    }

    bool null(){
        return head.null();
    }

    void pop_front() {
        if (!head.null()) {
            UnqPtr<NodeUnique<T>> oldHead = std::move(head);
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
    ~LinkedListUnique(){
        clear();
    }
};
