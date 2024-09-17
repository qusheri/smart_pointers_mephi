#pragma once

#include <cstddef>
#include <utility>

template<typename T>
class UnqPtr {
private:
    T *ptr;

public:
    explicit UnqPtr(T *p = nullptr) : ptr(p) {}
    ~UnqPtr() {
        delete ptr;
    }

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
    T &operator[]() const { return (*ptr)[]; }
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

    bool null(){
        return ptr == nullptr;
    }
};


template<typename T>
class UnqPtr<T[]> {
private:
    T *ptr;

public:
    explicit UnqPtr(T *p = nullptr) : ptr(p) {}
    ~UnqPtr() {
        delete[] ptr;
    }

    UnqPtr(const UnqPtr &) = delete;
    UnqPtr &operator=(const UnqPtr &) = delete;

    UnqPtr(UnqPtr &&other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UnqPtr &operator=(UnqPtr &&other) noexcept {
        if (this != &other) {
            delete[] ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T &operator*() const { return *ptr; }
    T *operator->() const { return ptr; }
    T &operator[]() const { return (*ptr)[]; }
    T *get() const { return ptr; }

    void reset(T *p = nullptr) {
        delete[] ptr;
        ptr = p;
    }

    T *release() {
        T *temp = ptr;
        ptr = nullptr;
        return temp;
    }

    bool null(){
        return ptr == nullptr;
    }
};

template<typename T>
class ShrdPtr {
private:
    T *ptr;
    size_t *referenceCount;

    void clean(){
        if (referenceCount && --(*referenceCount) == 0) {
            delete ptr;
            delete referenceCount;
        }
    }

public:
    explicit ShrdPtr(UnqPtr<T> *p = nullptr) : ptr(p->release()), referenceCount(new size_t(1)) {}

    ShrdPtr(const ShrdPtr &other)
            : ptr(other.ptr), referenceCount(other.referenceCount) {
        if (referenceCount) {
            ++(*referenceCount);
        }
    }

    ShrdPtr &operator=(const ShrdPtr &other) {
        if (this != &other) {
            if(referenceCount) {
                *referenceCount -= 1;
                if (*referenceCount == 0) {
                    delete ptr;
                    delete referenceCount;
                }
            }
            ptr = other.ptr;
            referenceCount = other.referenceCount;
            if (referenceCount) {
                ++(*referenceCount);
            }
        }
        return *this;
    }
    ~ShrdPtr() {
        clean();
    }

    T &operator*() const { return *ptr; }
    T *operator->() const { return ptr; }

    size_t use_count() const { return referenceCount ? *referenceCount : 0; }

    void reset(UnqPtr<T> *p = nullptr) {
        clean();
        ptr = p;
        referenceCount = new size_t(1);
    }

    bool null(){
        return ptr == nullptr;
    }
};


template<typename T>
class ShrdPtr<T[]> {
private:
    T *ptr;
    size_t *referenceCount;

    void clean(){
        if (referenceCount && --(*referenceCount) == 0) {
            delete[] ptr;
            delete referenceCount;
        }
    }

public:
    explicit ShrdPtr(UnqPtr<T> *p = nullptr) : ptr(p->release()), referenceCount(new size_t(1)) {}

    ShrdPtr(const ShrdPtr &other)
            : ptr(other.ptr), referenceCount(other.referenceCount) {
        if (referenceCount) {
            ++(*referenceCount);
        }
    }

    ShrdPtr &operator=(const ShrdPtr &other) {
        if (this != &other) {
            if(referenceCount) {
                *referenceCount -= 1;
                if (*referenceCount == 0) {
                    delete ptr;
                    delete referenceCount;
                }
            }
            ptr = other.ptr;
            referenceCount = other.referenceCount;
            if (referenceCount) {
                ++(*referenceCount);
            }
        }
        return *this;
    }
    ~ShrdPtr() {
        clean();
    }

    T &operator*() const { return *ptr; }
    T *operator->() const { return ptr; }

    size_t use_count() const { return referenceCount ? *referenceCount : 0; }

    void reset(UnqPtr<T> *p = nullptr) {
        clean();
        ptr = p;
        referenceCount = new size_t(1);
    }

    bool null(){
        return ptr == nullptr;
    }
};
