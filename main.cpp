#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "DataStore.h"

namespace {

int findAccountIndex(const std::vector<Account>& accounts, const std::string& username) {
    for (int index = 0; index < static_cast<int>(accounts.size()); index++) {
        if (accounts[index].username == username) return index;
    }
    return -1;
}

bool uidExists(const std::vector<Account>& accounts, const std::string& uid) {
    for (const Account& account : accounts) {
        if (account.uid == uid) return true;
    }
    return false;
}

int findUserId(const Graph& socialGraph, const std::vector<Account>& accounts, const std::string& searchTerm) {
    List<User> users = socialGraph.getAllUsers();
    for (int index = 0; index < users.size(); index++) {
        if (searchTerm == users[index].getName()) return users[index].getId();
    }

    for (const Account& account : accounts) {
        if (account.uid == searchTerm) return account.userId;
    }
    return -1;
}

const Account* findAccountByUserId(const std::vector<Account>& accounts, int userId) {
    for (const Account& account : accounts) {
        if (account.userId == userId) return &account;
    }
    return nullptr;
}

int readInteger(const char* prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "Please enter a valid number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void printUsers(const Graph& socialGraph, const std::vector<Account>& accounts) {
    List<User> users = socialGraph.getAllUsers();
    if (users.isEmpty()) {
        std::cout << "No users have been added yet.\n";
        return;
    }

    std::cout << "Users:\n";
    for (int index = 0; index < users.size(); index++) {
        const Account* account = findAccountByUserId(accounts, users[index].getId());
        std::cout << "  " << users[index];
        if (account != nullptr) std::cout << " (UID: " << account->uid << ')';
        std::cout << '\n';
    }
}

const Account* findAccount(const std::vector<Account>& accounts, const std::string& username) {
    for (const Account& account : accounts) {
        if (account.username == username) return &account;
    }
    return nullptr;
}

void printCurrentUser(const Graph& socialGraph, int currentUserId) {
    const User* user = socialGraph.findUser(currentUserId);
    if (user != nullptr) {
        std::cout << "Logged in as [" << user->getId() << "] " << user->getName() << "\n";
    }
}

void printUserIds(const Graph& socialGraph, const char* label, const List<int>& ids) {
    std::cout << label;
    if (ids.isEmpty()) {
        std::cout << "None\n";
        return;
    }

    for (int index = 0; index < ids.size(); index++) {
        const User* user = socialGraph.findUser(ids[index]);
        if (user != nullptr) {
            std::cout << '[' << user->getId() << "] " << user->getName();
        } else {
            std::cout << ids[index];
        }

        if (index + 1 < ids.size()) std::cout << ", ";
    }
    std::cout << '\n';
}

void printMenu() {
    std::cout << "\n1. Add friend\n"
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
    std::vector<Account> accounts;
    DataStore dataStore;
    int nextUserId = 1;
    bool running = true;
    dataStore.load(socialGraph, accounts, nextUserId);

    std::cout << "Welcome to the Mini Social Network.\n";

    while (running) {
        int currentUserId = -1;
        while (running && currentUserId == -1) {
            std::cout << "\n=== Welcome ===\n"
                      << "1. Register\n"
                      << "2. Login\n"
                      << "3. Delete account\n"
                      << "0. Exit\n";
            int choice = readInteger("Choose an option: ");

            if (choice == 1) {
                std::string username;
                std::string password;
                std::string displayName;
                std::string uid;

                std::cout << "Choose a username: ";
                std::getline(std::cin >> std::ws, username);
                std::cout << "Choose a password: ";
                std::getline(std::cin, password);
                std::cout << "Enter your display name: ";
                std::getline(std::cin, displayName);
                std::cout << "Choose your unique UID (letters and numbers are allowed): ";
                std::getline(std::cin, uid);

                if (username.empty() || password.empty() || displayName.empty() || uid.empty()) {
                    std::cout << "Username, password, display name, and UID are required.\n";
                } else if (findAccount(accounts, username) != nullptr) {
                    std::cout << "That username is already taken.\n";
                } else if (uidExists(accounts, uid)) {
                    std::cout << "That UID is already taken. Choose another one.\n";
                } else {
                    int userId = nextUserId++;
                    accounts.push_back({username, password, uid, userId});
                    socialGraph.addUser(userId, displayName.c_str());
                    if (dataStore.save(socialGraph, accounts)) {
                        std::cout << "Registration successful and saved. You can now log in.\n";
                    } else {
                        std::cout << "Registration successful, but the data file could not be saved.\n";
                    }
                }
            } else if (choice == 2) {
                std::string username;
                std::string password;
                std::cout << "Username: ";
                std::getline(std::cin >> std::ws, username);
                std::cout << "Password: ";
                std::getline(std::cin, password);

                const Account* account = findAccount(accounts, username);
                if (account == nullptr || account->password != password) {
                    std::cout << "Invalid username or password.\n";
                } else {
                    currentUserId = account->userId;
                    std::cout << "Login successful.\n";
                    printCurrentUser(socialGraph, currentUserId);
                }
            } else if (choice == 3) {
                std::string username;
                std::string password;
                std::cout << "Username: ";
                std::getline(std::cin >> std::ws, username);
                std::cout << "Password: ";
                std::getline(std::cin, password);

                int accountIndex = findAccountIndex(accounts, username);
                if (accountIndex == -1 || accounts[accountIndex].password != password) {
                    std::cout << "Invalid username or password. Account was not deleted.\n";
                } else {
                    int userId = accounts[accountIndex].userId;
                    socialGraph.removeUser(userId);
                    accounts.erase(accounts.begin() + accountIndex);
                    dataStore.save(socialGraph, accounts);
                    std::cout << "Account and all its friendships were deleted.\n";
                }
            } else if (choice == 0) {
                running = false;
                std::cout << "Goodbye.\n";
            } else {
                std::cout << "That option is not available.\n";
            }
        }

        while (running && currentUserId != -1) {
            printMenu();
            int choice = readInteger("Choose an option: ");

            switch (choice) {
            case 1: {
                std::string searchTerm;
                std::cout << "Enter the friend's display name or UID: ";
                std::getline(std::cin >> std::ws, searchTerm);
                int friendId = findUserId(socialGraph, accounts, searchTerm);
                if (friendId == -1) {
                    std::cout << "No user found with that display name or UID.\n";
                    break;
                }
                if (socialGraph.addFriendship(currentUserId, friendId)) {
                    dataStore.save(socialGraph, accounts);
                    std::cout << "Friend added and saved.\n";
                } else {
                    std::cout << "Could not add friend. Check the ID, duplicate friendship, or self-friendship.\n";
                }
                break;
            }
            case 2: {
                std::string searchTerm;
                std::cout << "Enter the friend's display name or UID: ";
                std::getline(std::cin >> std::ws, searchTerm);
                int friendId = findUserId(socialGraph, accounts, searchTerm);
                if (friendId == -1) {
                    std::cout << "No user found with that display name or UID.\n";
                    break;
                }
                if (socialGraph.removeFriendship(currentUserId, friendId)) {
                    dataStore.save(socialGraph, accounts);
                    std::cout << "Friend removed and saved.\n";
                } else {
                    std::cout << "That user is not your friend.\n";
                }
                break;
            }
            case 3:
                printUserIds(socialGraph, "Your friends: ", socialGraph.getFriends(currentUserId));
                break;
            case 4:
            {
                std::string searchTerm;
                std::cout << "Search by display name or UID (press Enter for all users): ";
                std::getline(std::cin, searchTerm);
                if (searchTerm.empty()) {
                    printUsers(socialGraph, accounts);
                    break;
                }

                int foundUserId = findUserId(socialGraph, accounts, searchTerm);
                if (foundUserId == -1) {
                    std::cout << "No user found with that display name or UID.\n";
                } else {
                    const User* user = socialGraph.findUser(foundUserId);
                    const Account* account = findAccountByUserId(accounts, foundUserId);
                    std::cout << "Found: [" << user->getId() << "] " << user->getName();
                    if (account != nullptr) std::cout << " (UID: " << account->uid << ')';
                    std::cout << '\n';
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
                std::string searchTerm;
                std::cout << "Enter the other user's display name or UID: ";
                std::getline(std::cin >> std::ws, searchTerm);
                int otherUserId = findUserId(socialGraph, accounts, searchTerm);
                if (otherUserId == -1) {
                    std::cout << "No user found with that display name or UID.\n";
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
                std::cout << "You have been logged out.\n";
                break;
            case 0:
                running = false;
                std::cout << "Goodbye.\n";
                break;
            default:
                std::cout << "That option is not available.\n";
                break;
            }
        }
    }

    dataStore.save(socialGraph, accounts);
    return 0;
}
