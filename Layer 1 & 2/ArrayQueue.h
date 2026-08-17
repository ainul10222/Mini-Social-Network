#ifndef ARRAYQUEUE_H
#define ARRAYQUEUE_H


template <typename T>
class ArrayQueue {
public:
    explicit ArrayQueue(int initialCapacity = 4)
        : data(nullptr), frontIdx(0), count(0), capacity(initialCapacity < 1 ? 1 : initialCapacity) {
        data = new T[capacity];
    }

    ArrayQueue(const ArrayQueue<T>& other) : data(nullptr), frontIdx(0), count(0), capacity(1) {
        copyFrom(other);
    }

    ArrayQueue<T>& operator=(const ArrayQueue<T>& other) {
        if (this == &other) return *this;
        delete[] data;
        copyFrom(other);
        return *this;
    }

    ~ArrayQueue() {
        delete[] data;
    }

    void enqueue(const T& value) {
        if (count == capacity) {
            resize(capacity * 2);
        }
        int rear = (frontIdx + count) % capacity;
        data[rear] = value;
        count++;
    }

    T dequeue() {
        T value = data[frontIdx];
        frontIdx = (frontIdx + 1) % capacity;
        count--;
        return value;
    }

    const T& front() const {
        return data[frontIdx];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

private:
    T* data;
    int frontIdx;
    int count;
    int capacity;

    void resize(int newCapacity) {
        T* bigger = new T[newCapacity];
        for (int i = 0; i < count; i++) {
            bigger[i] = data[(frontIdx + i) % capacity];
        }
        delete[] data;
        data = bigger;
        frontIdx = 0;
        capacity = newCapacity;
    }

    void copyFrom(const ArrayQueue<T>& other) {
        capacity = other.capacity;
        count = other.count;
        frontIdx = 0;
        data = new T[capacity];
        for (int i = 0; i < count; i++) {
            data[i] = other.data[(other.frontIdx + i) % other.capacity];
        }
    }
};

#endif // ARRAYQUEUE_H
