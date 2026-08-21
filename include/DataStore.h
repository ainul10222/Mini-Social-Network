#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <vector>

#include "Graph.h"

using namespace std;

struct Account {
    string username;
    string password;
    string uid;
    int userId;
};

class DataStore {
public:
    explicit DataStore(const string& fileName = "Persistence/social_network.txt");

    bool load(Graph& socialGraph, vector<Account>& accounts, int& nextUserId) const;
    bool save(const Graph& socialGraph, const vector<Account>& accounts) const;

private:
    string fileName;
};

#endif // DATASTORE_H
