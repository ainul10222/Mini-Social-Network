#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include "List.h"

using namespace std;

template <typename T>
class LinkedList {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& v, Node* n = nullptr) : value(v), next(n) {}
    };

    Node* head;
    int count;

    void copyFrom(const LinkedList<T>& other) {
        head = nullptr;
        count = 0;

        Node* otherCur = other.head;
        Node* myTail = nullptr;
        while (otherCur != nullptr) {
            Node* newNode = new Node(otherCur->value);
            if (myTail == nullptr) {
                head = newNode;
            } else {
                myTail->next = newNode;
            }
            myTail = newNode;
            count++;
            otherCur = otherCur->next;
        }
    }

    void clear() {
        Node* cur = head;
        while (cur != nullptr) {
            Node* doomed = cur;
            cur = cur->next;
            delete doomed;
        }
        head = nullptr;
        count = 0;
    }

public:
    LinkedList() : head(nullptr), count(0) {}

    LinkedList(const LinkedList<T>& other) : head(nullptr), count(0) {
        copyFrom(other);
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this == &other) return *this;
        clear();
        copyFrom(other);
        return *this;
    }

    ~LinkedList() {
        clear();
    }


    void insertFront(const T& value) {
        head = new Node(value, head);
        count++;
    }

    bool remove(const T& value) {
        Node* cur = head;
        Node* prev = nullptr;
        while (cur != nullptr) {
            if (cur->value == value) {
                if (prev == nullptr) {
                    head = cur->next;
                } else {
                    prev->next = cur->next;
                }
                delete cur;
                count--;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    bool contains(const T& value) const {
        Node* cur = head;
        while (cur != nullptr) {
            if (cur->value == value) return true;
            cur = cur->next;
        }
        return false;
    }

    int size() const { return count; }
    bool isEmpty() const { return count == 0; }

    // Converts to an array-based List<T> so callers (Graph, main.cpp) can
    // hand results back / index into them without ever seeing a Node.
    List<T> toList() const {
        List<T> result(count > 0 ? count : 1);
        Node* cur = head;
        while (cur != nullptr) {
            result.add(cur->value);
            cur = cur->next;
        }
        return result;
    }

    template <typename U>
    friend ostream& operator<<(ostream& out, const LinkedList<U>& list);
};

template <typename U>
ostream& operator<<(ostream& out, const LinkedList<U>& list) {
    out << "( ";
    typename LinkedList<U>::Node* cur = list.head;
    while (cur != nullptr) {
        out << cur->value;
        if (cur->next != nullptr) out << " -> ";
        cur = cur->next;
    }
    out << " )";
    return out;
}

#endif // LINKEDLIST_H
