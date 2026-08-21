#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "DataStore.h"

using namespace std;

namespace {

int findAccountIndex(const vector<Account>& accounts, const string& username) {
    for (int index = 0; index < static_cast<int>(accounts.size()); index++) {
        if (accounts[index].username == username) return index;
    }
    return -1;
}

bool uidExists(const vector<Account>& accounts, const string& uid) {
    for (const Account& account : accounts) {
        if (account.uid == uid) return true;
    }
    return false;
}

int findUserId(const Graph& socialGraph, const vector<Account>& accounts, const string& searchTerm) {
    List<User> users = socialGraph.getAllUsers();
    for (int index = 0; index < users.size(); index++) {
        if (searchTerm == users[index].getName()) return users[index].getId();
    }

    for (const Account& account : accounts) {
        if (account.uid == searchTerm) return account.userId;
    }
    return -1;
}

const Account* findAccountByUserId(const vector<Account>& accounts, int userId) {
    for (const Account& account : accounts) {
        if (account.userId == userId) return &account;
    }
    return nullptr;
}

int readInteger(const char* prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Please enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void printUsers(const Graph& socialGraph, const vector<Account>& accounts) {
    List<User> users = socialGraph.getAllUsers();
    if (users.isEmpty()) {
        cout << "No users have been added yet.\n";
        return;
    }

    cout << "Users:\n";
    for (int index = 0; index < users.size(); index++) {
        const Account* account = findAccountByUserId(accounts, users[index].getId());
        cout << "  " << users[index];
        if (account != nullptr) cout << " (UID: " << account->uid << ')';
        cout << '\n';
    }
}

const Account* findAccount(const vector<Account>& accounts, const string& username) {
    for (const Account& account : accounts) {
        if (account.username == username) return &account;
    }
    return nullptr;
}

void printCurrentUser(const Graph& socialGraph, int currentUserId) {
    const User* user = socialGraph.findUser(currentUserId);
    if (user != nullptr) {
        cout << "Logged in as [" << user->getId() << "] " << user->getName() << "\n";
    }
}

void printUserIds(const Graph& socialGraph, const char* label, const List<int>& ids) {
    cout << label;
    if (ids.isEmpty()) {
        cout << "None\n";
        return;
    }

    for (int index = 0; index < ids.size(); index++) {
        const User* user = socialGraph.findUser(ids[index]);
        if (user != nullptr) {
            cout << '[' << user->getId() << "] " << user->getName();
        } else {
            cout << ids[index];
        }

        if (index + 1 < ids.size()) cout << ", ";
    }
    cout << '\n';
}

void printMenu() {
    cout << "\n1. Add friend\n"
              << "2. Remove friend\n"
              << "3. Show my friends\n"
              << "4. Find users\n"
              << "5. Run BFS from me\n"
              << "6. Run DFS from me\n"
              << "7. Show mutual friends\n"
              << "8. Suggest friends\n"
              << "9. Logout\n"
              << "0. Exit\n";
}

}

int main() {
    Graph socialGraph;
    vector<Account> accounts;
    DataStore dataStore;
    int nextUserId = 1;
    bool running = true;
    dataStore.load(socialGraph, accounts, nextUserId);

    cout << "Welcome to the Mini Social Network.\n";

    while (running) {
        int currentUserId = -1;
        while (running && currentUserId == -1) {
            cout << "\n=== Welcome ===\n"
                      << "1. Register\n"
                      << "2. Login\n"
                      << "3. Delete account\n"
                      << "0. Exit\n";
            int choice = readInteger("Choose an option: ");

            if (choice == 1) {
                string username;
                string password;
                string displayName;
                string uid;

                cout << "Choose a username: ";
                getline(cin >> ws, username);
                cout << "Choose a password: ";
                getline(cin, password);
                cout << "Enter your display name: ";
                getline(cin, displayName);
                cout << "Choose your unique UID (letters and numbers are allowed): ";
                getline(cin, uid);

                if (username.empty() || password.empty() || displayName.empty() || uid.empty()) {
                    cout << "Username, password, display name, and UID are required.\n";
                } else if (findAccount(accounts, username) != nullptr) {
                    cout << "That username is already taken.\n";
                } else if (uidExists(accounts, uid)) {
                    cout << "That UID is already taken. Choose another one.\n";
                } else {
                    int userId = nextUserId++;
                    accounts.push_back({username, password, uid, userId});
                    socialGraph.addUser(userId, displayName.c_str());
                    if (dataStore.save(socialGraph, accounts)) {
                        cout << "Registration successful and saved. You can now log in.\n";
                    } else {
                        cout << "Registration successful, but the data file could not be saved.\n";
                    }
                }
            } else if (choice == 2) {
                string username;
                string password;
                cout << "Username: ";
                getline(cin >> ws, username);
                cout << "Password: ";
                getline(cin, password);

                const Account* account = findAccount(accounts, username);
                if (account == nullptr || account->password != password) {
                    cout << "Invalid username or password.\n";
                } else {
                    currentUserId = account->userId;
                    cout << "Login successful.\n";
                    printCurrentUser(socialGraph, currentUserId);
                }
            } else if (choice == 3) {
                string username;
                string password;
                cout << "Username: ";
                getline(cin >> ws, username);
                cout << "Password: ";
                getline(cin, password);

                int accountIndex = findAccountIndex(accounts, username);
                if (accountIndex == -1 || accounts[accountIndex].password != password) {
                    cout << "Invalid username or password. Account was not deleted.\n";
                } else {
                    int userId = accounts[accountIndex].userId;
                    socialGraph.removeUser(userId);
                    accounts.erase(accounts.begin() + accountIndex);
                    dataStore.save(socialGraph, accounts);
                    cout << "Account and all its friendships were deleted.\n";
                }
            } else if (choice == 0) {
                running = false;
                cout << "Goodbye.\n";
            } else {
                cout << "That option is not available.\n";
            }
        }

        while (running && currentUserId != -1) {
            printMenu();
            int choice = readInteger("Choose an option: ");

            switch (choice) {
            case 1: {
                string searchTerm;
                cout << "Enter the friend's display name or UID: ";
                getline(cin >> ws, searchTerm);
                int friendId = findUserId(socialGraph, accounts, searchTerm);
                if (friendId == -1) {
                    cout << "No user found with that display name or UID.\n";
                    break;
                }
                if (socialGraph.addFriendship(currentUserId, friendId)) {
                    dataStore.save(socialGraph, accounts);
                    cout << "Friend added and saved.\n";
                } else {
                    cout << "Could not add friend. Check the ID, duplicate friendship, or self-friendship.\n";
                }
                break;
            }
            case 2: {
                string searchTerm;
                cout << "Enter the friend's display name or UID: ";
                getline(cin >> ws, searchTerm);
                int friendId = findUserId(socialGraph, accounts, searchTerm);
                if (friendId == -1) {
                    cout << "No user found with that display name or UID.\n";
                    break;
                }
                if (socialGraph.removeFriendship(currentUserId, friendId)) {
                    dataStore.save(socialGraph, accounts);
                    cout << "Friend removed and saved.\n";
                } else {
                    cout << "That user is not your friend.\n";
                }
                break;
            }
            case 3:
                printUserIds(socialGraph, "Your friends: ", socialGraph.getFriends(currentUserId));
                break;
            case 4:
            {
                string searchTerm;
                cout << "Search by display name or UID (press Enter for all users): ";
                getline(cin, searchTerm);
                if (searchTerm.empty()) {
                    printUsers(socialGraph, accounts);
                    break;
                }

                int foundUserId = findUserId(socialGraph, accounts, searchTerm);
                if (foundUserId == -1) {
                    cout << "No user found with that display name or UID.\n";
                } else {
                    const User* user = socialGraph.findUser(foundUserId);
                    const Account* account = findAccountByUserId(accounts, foundUserId);
                    cout << "Found: [" << user->getId() << "] " << user->getName();
                    if (account != nullptr) cout << " (UID: " << account->uid << ')';
                    cout << '\n';
                }
                break;
            }
            case 5:
                printUserIds(socialGraph, "BFS order: ", socialGraph.bfsTraversal(currentUserId));
                break;
            case 6:
                printUserIds(socialGraph, "DFS order: ", socialGraph.dfsTraversal(currentUserId));
                break;
            case 7: {
                string searchTerm;
                cout << "Enter the other user's display name or UID: ";
                getline(cin >> ws, searchTerm);
                int otherUserId = findUserId(socialGraph, accounts, searchTerm);
                if (otherUserId == -1) {
                    cout << "No user found with that display name or UID.\n";
                    break;
                }
                printUserIds(socialGraph, "Mutual friends: ", socialGraph.getMutualFriends(currentUserId, otherUserId));
                break;
            }
            case 8: {
                int limit = readInteger("How many suggestions should be shown? ");
                printUserIds(socialGraph, "Friend suggestions: ", socialGraph.suggestFriends(currentUserId, limit));
                break;
            }
            case 9:
                currentUserId = -1;
                cout << "You have been logged out.\n";
                break;
            case 0:
                running = false;
                cout << "Goodbye.\n";
                break;
            default:
                cout << "That option is not available.\n";
                break;
            }
        }
    }

    dataStore.save(socialGraph, accounts);
    return 0;
}
