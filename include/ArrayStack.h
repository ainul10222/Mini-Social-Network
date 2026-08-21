#ifndef ARRAYSTACK_H
#define ARRAYSTACK_H

using namespace std;

template <typename T>
class ArrayStack {
public:
    explicit ArrayStack(int initialCapacity = 4)
        : data(nullptr), top(-1), capacity(initialCapacity < 1 ? 1 : initialCapacity) {
        data = new T[capacity];
    }

    ArrayStack(const ArrayStack<T>& other) : data(nullptr), top(-1), capacity(1) {
        copyFrom(other);
    }

    ArrayStack<T>& operator=(const ArrayStack<T>& other) {
        if (this == &other) return *this;
        delete[] data;
        copyFrom(other);
        return *this;
    }

    ~ArrayStack() {
        delete[] data;
    }

    void push(const T& value) {
        if (top + 1 == capacity) {
            resize(capacity * 2);
        }
        top++;
        data[top] = value;
    }

    T pop() {
        T value = data[top];
        top--;
        return value;
    }

    const T& peek() const {
        return data[top];
    }

    bool isEmpty() const { return top == -1; }
    int size() const { return top + 1; }

private:
    T* data;
    int top;
    int capacity;

    void resize(int newCapacity) {
        T* bigger = new T[newCapacity];
        for (int i = 0; i <= top; i++) bigger[i] = data[i];
        delete[] data;
        data = bigger;
        capacity = newCapacity;
    }

    void copyFrom(const ArrayStack<T>& other) {
        capacity = other.capacity;
        top = other.top;
        data = new T[capacity];
        for (int i = 0; i <= top; i++) data[i] = other.data[i];
    }
};

#endif // ARRAYSTACK_H
