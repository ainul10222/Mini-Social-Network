#ifndef GRAPH_H
#define GRAPH_H

#include "User.h"
#include "List.h"
#include "LinkedList.h"

using namespace std;

class Graph {
public:
    explicit Graph(int initialCapacity = 4);
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);
    ~Graph();

    bool addUser(int id, const char* name);
    bool removeUser(int id);
    bool userExists(int id) const;

    bool addFriendship(int id1, int id2);
    bool removeFriendship(int id1, int id2);
    bool areFriends(int id1, int id2) const;

    List<int> bfsTraversal(int startId) const;
    List<int> dfsTraversal(int startId) const;

    List<int> getMutualFriends(int id1, int id2) const;
    List<int> suggestFriends(int id, int maxSuggestions = 5) const;

    List<int> getFriends(int id) const;
    List<User> getAllUsers() const;
    const User* findUser(int id) const;

private:
    User* users;
    LinkedList<int>* friends;
    int count;
    int capacity;

    void resize(int newCapacity);
    int findIndex(int id) const;
    int findInsertPos(int id) const;

    void copyFrom(const Graph& other);
    void destroy();
};

#endif // GRAPH_H
