#pragma once
#include <cstddef>
#include <utility>

template<typename T>
class UnqPtr {
private:
    T *ptr;

public:
    explicit UnqPtr(T *p = nullptr) : ptr(p) {}
    ~UnqPtr() { delete ptr; }

    UnqPtr(const UnqPtr &) = delete;
    UnqPtr &operator=(const UnqPtr &) = delete;

    UnqPtr(UnqPtr &&other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UnqPtr &operator=(UnqPtr &&other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T &operator*() const { return *ptr; }
    T *operator->() const { return ptr; }
    T *get() const { return ptr; }

    void reset(T *p = nullptr) {
        delete ptr;
        ptr = p;
    }

    T *release() {
        T *temp = ptr;
        ptr = nullptr;
        return temp;
    }
};

template<typename T>
class ShrdPtr {
private:
    UnqPtr<T> *uptr;
    size_t *referenceCount;

public:
    explicit ShrdPtr(UnqPtr<T> *p = nullptr)
            : uptr(p), referenceCount(new size_t(1)) {}

    ShrdPtr(const ShrdPtr &other)
            : uptr(other.uptr), referenceCount(other.referenceCount) {
        if (referenceCount) {
            ++(*referenceCount);
        }
    }

    ShrdPtr &operator=(const ShrdPtr &other) {
        if (this != &other) {
            if (referenceCount && --(*referenceCount) == 0) {
                delete uptr;
                delete referenceCount;
            }
            uptr = other.uptr;
            referenceCount = other.referenceCount;
            if (referenceCount) {
                ++(*referenceCount);
            }
        }
        return *this;
    }

    ~ShrdPtr() {
        if (referenceCount && --(*referenceCount) == 0) {
            uptr = nullptr;
            delete referenceCount;
        }
    }

    T &operator*() const { return *uptr->get(); }
    T *operator->() const { return uptr->get(); }

    size_t use_count() const { return referenceCount ? *referenceCount : 0; }

    void reset(UnqPtr<T> *p = nullptr) {
        if (referenceCount && --(*referenceCount) == 0) {
            delete uptr;
            delete referenceCount;
        }
        uptr = p;
        referenceCount = new size_t(1);
    }
};
