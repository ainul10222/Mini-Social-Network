#include "Graph.h"
#include "ArrayQueue.h"
#include "ArrayStack.h"


Graph::Graph(int initialCapacity)
    : users(nullptr), friends(nullptr), count(0),
      capacity(initialCapacity < 1 ? 1 : initialCapacity) {
    users = new User[capacity];
    friends = new LinkedList<int>[capacity];
}

Graph::Graph(const Graph& other) : users(nullptr), friends(nullptr), count(0), capacity(1) {
    copyFrom(other);
}

Graph& Graph::operator=(const Graph& other) {
    if (this == &other) return *this;
    destroy();
    copyFrom(other);
    return *this;
}

Graph::~Graph() {
    destroy();
}

void Graph::destroy() {
    delete[] users;
    delete[] friends;
    users = nullptr;
    friends = nullptr;
}

void Graph::copyFrom(const Graph& other) {
    capacity = other.capacity;
    count = other.count;
    users = new User[capacity];
    friends = new LinkedList<int>[capacity];
    for (int i = 0; i < count; i++) {
        users[i] = other.users[i];
        friends[i] = other.friends[i];
    }
}

void Graph::resize(int newCapacity) {
    User* biggerUsers = new User[newCapacity];
    LinkedList<int>* biggerFriends = new LinkedList<int>[newCapacity];
    for (int i = 0; i < count; i++) {
        biggerUsers[i] = users[i];
        biggerFriends[i] = friends[i];
    }
    delete[] users;
    delete[] friends;
    users = biggerUsers;
    friends = biggerFriends;
    capacity = newCapacity;
}

int Graph::findIndex(int id) const {
    int lo = 0, hi = count - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (users[mid].getId() == id) return mid;
        if (users[mid].getId() < id) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

int Graph::findInsertPos(int id) const {
    int lo = 0, hi = count;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (users[mid].getId() < id) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

const User* Graph::findUser(int id) const {
    int idx = findIndex(id);
    return (idx == -1) ? nullptr : &users[idx];
}

bool Graph::userExists(int id) const {
    return findIndex(id) != -1;
}

bool Graph::addUser(int id, const char* name) {
    if (userExists(id)) return false;

    if (count == capacity) {
        resize(capacity * 2);
    }

    int pos = findInsertPos(id);
    for (int i = count; i > pos; i--) {
        users[i] = users[i - 1];
        friends[i] = friends[i - 1];
    }

    users[pos] = User(id, name);
    friends[pos] = LinkedList<int>();
    count++;
    return true;
}

bool Graph::removeUser(int id) {
    int idx = findIndex(id);
    if (idx == -1) return false;

    for (int i = 0; i < count; i++) {
        if (i != idx) friends[i].remove(id);
    }

    for (int i = idx; i < count - 1; i++) {
        users[i] = users[i + 1];
        friends[i] = friends[i + 1];
    }
    count--;
    return true;
}

bool Graph::addFriendship(int id1, int id2) {
    if (id1 == id2)
        return false;
    int i1 = findIndex(id1);
    int i2 = findIndex(id2);
    if (i1 == -1 || i2 == -1)
        return false;
    if (friends[i1].contains(id2))
     return false;

    friends[i1].insertFront(id2);
    friends[i2].insertFront(id1);
    return true;
}

bool Graph::removeFriendship(int id1, int id2) {
    int i1 = findIndex(id1);
    int i2 = findIndex(id2);
    if (i1 == -1 || i2 == -1) return false;

    bool removed1 = friends[i1].remove(id2);
    bool removed2 = friends[i2].remove(id1);
    return removed1 && removed2;
}

bool Graph::areFriends(int id1, int id2) const {
    int i1 = findIndex(id1);
    int i2 = findIndex(id2);
    if (i1 == -1 || i2 == -1) return false;
    return friends[i1].contains(id2);
}


List<int> Graph::bfsTraversal(int startId) const {
    List<int> order;
    int startIdx = findIndex(startId);
    if (startIdx == -1) return order;

    bool* visited = new bool[capacity]();
    ArrayQueue<int> q;

    visited[startIdx] = true;
    q.enqueue(startId);

    while (!q.isEmpty()) {
        int currentId = q.dequeue();
        order.add(currentId);

        int currentIdx = findIndex(currentId);
        List<int> neighbors = friends[currentIdx].toList();
        for (int i = 0; i < neighbors.size(); i++) {
            int neighborId = neighbors[i];
            int neighborIdx = findIndex(neighborId);
            if (!visited[neighborIdx]) {
                visited[neighborIdx] = true;
                q.enqueue(neighborId);
            }
        }
    }

    delete[] visited;
    return order;
}

List<int> Graph::dfsTraversal(int startId) const {
    List<int> order;
    if (!userExists(startId)) return order;

    bool* visited = new bool[capacity]();
    ArrayStack<int> s;

    s.push(startId);

    while (!s.isEmpty()) {
        int currentId = s.pop();
        int currentIdx = findIndex(currentId);

        if (visited[currentIdx]) continue;
        visited[currentIdx] = true;
        order.add(currentId);

        List<int> neighbors = friends[currentIdx].toList();
        for (int i = 0; i < neighbors.size(); i++) {
            int neighborId = neighbors[i];
            int neighborIdx = findIndex(neighborId);
            if (!visited[neighborIdx]) {
                s.push(neighborId);
            }
        }
    }

    delete[] visited;
    return order;
}


List<int> Graph::getMutualFriends(int id1, int id2) const {
    List<int> result;
    int i1 = findIndex(id1);
    int i2 = findIndex(id2);
    if (i1 == -1 || i2 == -1) return result;

    List<int> f1 = friends[i1].toList();
    List<int> f2 = friends[i2].toList();

    for (int i = 0; i < f1.size(); i++) {
        if (f2.contains(f1[i])) {
            result.add(f1[i]);
        }
    }
    return result;
}

List<int> Graph::suggestFriends(int id, int maxSuggestions) const {
    List<int> result;
    int idx = findIndex(id);
    if (idx == -1) return result;

    List<int> direct = friends[idx].toList();

    for (int i = 0; i < direct.size() && result.size() < maxSuggestions; i++) {
        int friendId = direct[i];
        int friendIdx = findIndex(friendId);
        List<int> friendOfFriend = friends[friendIdx].toList();

        for (int j = 0; j < friendOfFriend.size() && result.size() < maxSuggestions; j++) {
            int candidate = friendOfFriend[j];
            if (candidate == id) continue;
            if (direct.contains(candidate)) continue;
            if (result.contains(candidate)) continue;
            result.add(candidate);
        }
    }
    return result;
}


List<int> Graph::getFriends(int id) const {
    int idx = findIndex(id);
    if (idx == -1)
        return List<int>();
    return friends[idx].toList();
}

List<User> Graph::getAllUsers() const {
    List<User> result(count > 0 ? count : 1);
    for (int i = 0; i < count; i++) {
        result.add(users[i]);
    }
    return result;
}
