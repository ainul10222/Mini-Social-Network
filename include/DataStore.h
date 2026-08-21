#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <vector>

#include "Graph.h"

struct Account {
    std::string username;
    std::string password;
    std::string uid;
    int userId;
};

class DataStore {
public:
    explicit DataStore(const std::string& fileName = "Persistence/social_network.txt");

    bool load(Graph& socialGraph, std::vector<Account>& accounts, int& nextUserId) const;
    bool save(const Graph& socialGraph, const std::vector<Account>& accounts) const;

private:
    std::string fileName;
};

#endif // DATASTORE_H
