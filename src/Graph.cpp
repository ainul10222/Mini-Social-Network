#include "Graph.h"
#include <queue>
#include <set>
#include <algorithm>
using namespace std;

Graph::Graph() {
    // Nothing to initialize — containers default-construct empty.
}

bool Graph::addUser(int id, const string& name) {
    // TODO: if userExists(id), return false.
    // Otherwise insert into `users` and create an empty entry in `adjacencyList`.
    return false;
}

bool Graph::removeUser(int id) {
    // TODO: erase id from `users`.
    // Then for every OTHER user, remove id from their adjacency list.
    // Then erase id's own adjacency list entry.
    return false;
}

bool Graph::userExists(int id) const {
    // TODO: check if `users` contains this id.
    return false;
}

bool Graph::addFriendship(int id1, int id2) {
    // TODO: check both users exist and aren't already friends.
    // Push id2 into adjacencyList[id1] AND id1 into adjacencyList[id2].
    return false;
}

bool Graph::removeFriendship(int id1, int id2) {
    // TODO: erase id2 from adjacencyList[id1]'s vector,
    // AND erase id1 from adjacencyList[id2]'s vector.
    return false;
}

bool Graph::areFriends(int id1, int id2) const {
    // TODO: check if id2 appears in adjacencyList.at(id1)
    return false;
}

vector<int> Graph::bfsTraversal(int startId) const {
    // TODO: classic BFS using queue and a set (or
    // unordered_set) of visited ids. Push startId, mark visited,
    // then repeatedly pop, record, and push unvisited neighbors.
    return {};
}

vector<int> Graph::dfsTraversal(int startId) const {
    // TODO: classic DFS. Can be done recursively (helper function)
    // or iteratively with an explicit stack. Track visited
    // the same way as BFS.
    return {};
}

vector<int> Graph::getMutualFriends(int id1, int id2) const {
    // TODO: get friends of id1, get friends of id2, return the
    // intersection. set_intersection works well if both
    // friend lists are sorted first.
    return {};
}

vector<int> Graph::suggestFriends(int id, int maxSuggestions) const {
    // TODO: run BFS from id but only go 2 levels deep.
    // Collect users at exactly distance 2 (friends of friends)
    // who are not already in adjacencyList[id]. Cap at maxSuggestions.
    return {};
}

vector<int> Graph::getFriends(int id) const {
    // TODO: return adjacencyList.at(id) if it exists, else {}
    return {};
}

vector<User> Graph::getAllUsers() const {
    // TODO: collect all values from the `users` map into a vector
    return {};
}
