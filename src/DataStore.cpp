#include "DataStore.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

DataStore::DataStore(const string& fileName) : fileName(fileName) {}

bool DataStore::save(const Graph& socialGraph, const vector<Account>& accounts) const {
    ofstream output(fileName);
    if (!output) return false;

    output << "ACCOUNTS\n";
    for (const Account& account : accounts) {
                output << quoted(account.username) << ' '
                             << quoted(account.password) << ' '
                         << quoted(account.uid) << ' '
               << account.userId << '\n';
    }

    output << "USERS\n";
    List<User> users = socialGraph.getAllUsers();
    for (int index = 0; index < users.size(); index++) {
        output << users[index].getId() << ' '
               << quoted(string(users[index].getName())) << '\n';
    }

    output << "FRIENDSHIPS\n";
    for (int userIndex = 0; userIndex < users.size(); userIndex++) {
        int userId = users[userIndex].getId();
        List<int> friends = socialGraph.getFriends(userId);
        for (int friendIndex = 0; friendIndex < friends.size(); friendIndex++) {
            int friendId = friends[friendIndex];
            if (userId < friendId) {
                output << userId << ' ' << friendId << '\n';
            }
        }
    }

    return true;
}

bool DataStore::load(Graph& socialGraph, vector<Account>& accounts, int& nextUserId) const {
    ifstream input(fileName);
    if (!input) return true;

    string lineText;
    int section = 0;
    int highestUserId = 0;

    while (getline(input, lineText)) {
        if (lineText == "ACCOUNTS") {
            section = 1;
            continue;
        }
        if (lineText == "USERS") {
            section = 2;
            continue;
        }
        if (lineText == "FRIENDSHIPS") {
            section = 3;
            continue;
        }
        if (lineText.empty()) continue;

        istringstream line(lineText);
        if (section == 1) {
            Account account;
            if (line >> quoted(account.username)
                     >> quoted(account.password)
                     >> quoted(account.uid)
                     >> account.userId) {
                accounts.push_back(account);
            }
        } else if (section == 2) {
            int firstValue;
            if (!(line >> firstValue)) continue;

            string name;
            if (line >> quoted(name)) {
                if (socialGraph.addUser(firstValue, name.c_str()) && firstValue > highestUserId) {
                    highestUserId = firstValue;
                }
            }
        } else if (section == 3) {
            int firstValue;
            int secondValue;
            if (line >> firstValue >> secondValue) {
                socialGraph.addFriendship(firstValue, secondValue);
            }
        }
    }

    nextUserId = highestUserId + 1;
    return true;
}
