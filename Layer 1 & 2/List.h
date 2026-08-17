#ifndef LIST_H
#define LIST_H

#include <iostream>


template <typename T>
class List {
public:
    explicit List(int initialCapacity = 4)
        : data(nullptr), count(0), capacity(initialCapacity < 1 ? 1 : initialCapacity) {
        data = new T[capacity];
    }

    List(const List<T>& other) : data(nullptr), count(0), capacity(1){
        copyFrom(other);
    }

    List<T>& operator=(const List<T>& other){
        if (this == &other) return *this;
        delete[] data;
        copyFrom(other);
        return *this;
    }

    ~List() {
        delete[] data;
    }

    void add(const T& item){
        if (count == capacity) {
            resize(capacity * 2);
        }
        data[count] = item;
        count++;
    }
    bool remove(const T& item){
        int idx = indexOf(item);
        if (idx == -1) return false;
        for (int i = idx; i < count - 1; i++) {
            data[i] = data[i + 1];
        }
        count--;
        return true;
    }

    bool contains(const T& item) const{
        return indexOf(item) != -1;
    }

    int indexOf(const T& item) const {
        for (int i = 0; i < count; i++){
            if (data[i] == item) return i;
        }
        return -1;
    }

    int size() const { return count; }
    bool isEmpty() const { return count == 0; }

    T& operator[](int index) { return data[index]; }
    const T& operator[](int index) const { return data[index]; }

    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const List<U>& list);

private:
    T* data;
    int count;
    int capacity;

    void resize(int newCapacity) {
        T* bigger = new T[newCapacity];
        for (int i = 0; i < count; i++) {
            bigger[i] = data[i];
        }
        delete[] data;
        data = bigger;
        capacity = newCapacity;
    }

    void copyFrom(const List<T>& other) {
        capacity = other.capacity;
        count = other.count;
        data = new T[capacity];
        for (int i = 0; i < count; i++) {
            data[i] = other.data[i];
        }
    }
};

template <typename U>
std::ostream& operator<<(std::ostream& out, const List<U>& list) {
    out << "{ ";
    for (int i = 0; i < list.count; i++) {
        out << list.data[i];
        if (i != list.count - 1) out << ", ";
    }
    out << " }";
    return out;
}

#endif
