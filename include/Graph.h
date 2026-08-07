#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>
#include <string>
#include "User.h"
using namespace std;

// Represents the social network as an undirected, unweighted graph.
// Vertices = Users (identified by int id). Edges = friendships.
class Graph {
public:
    Graph();

    // ---- User management ----

    // Adds a new user. Returns false if the id already exists.
    bool addUser(int id, const string& name);

    // Removes a user and every friendship connected to them.
    bool removeUser(int id);

    // Returns true if a user with this id exists in the graph.
    bool userExists(int id) const;

    // ---- Friendship management ----

    // Creates an undirected edge between two users.
    // Must add id2 to id1's adjacency list AND id1 to id2's.
    bool addFriendship(int id1, int id2);

    // Removes the undirected edge between two users.
    bool removeFriendship(int id1, int id2);

    // Returns true if id1 and id2 are directly connected.
    bool areFriends(int id1, int id2) const;

    // ---- Traversal ----

    // Breadth-first traversal starting from startId.
    // Returns the order in which users were visited.
    vector<int> bfsTraversal(int startId) const;

    // Depth-first traversal starting from startId.
    // Returns the order in which users were visited.
    vector<int> dfsTraversal(int startId) const;

    // ---- Analysis ----

    // Returns the set of users that are friends with BOTH id1 and id2.
    vector<int> getMutualFriends(int id1, int id2) const;

    // Suggests new friends for a user: friends-of-friends who
    // are not already direct friends, found via BFS at depth 2.
    vector<int> suggestFriends(int id, int maxSuggestions = 5) const;

    // Returns the direct friends list of a user.
    vector<int> getFriends(int id) const;

    // Returns every user currently in the network.
    vector<User> getAllUsers() const;

private:
    // userId -> list of directly connected friend userIds
    unordered_map<int, vector<int>> adjacencyList;

    // userId -> User object (name lookup)
    unordered_map<int, User> users;
};

#endif // GRAPH_H
