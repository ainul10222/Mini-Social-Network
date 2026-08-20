#include <iostream>
#include "Graph.h"

using namespace std;

void printList(const List<int>& list)
{
    cout << "{ ";

    for (int i = 0; i < list.size(); i++)
    {
        cout << list[i];

        if (i != list.size() - 1)
        {
            cout << ", ";
        }
    }

    cout << " }" << endl;
}

void printUsers(const List<User>& users)
{
    cout << "{ ";

    for (int i = 0; i < users.size(); i++)
    {
        cout << "[" << users[i].getId() << "] "
             << users[i].getName();

        if (i != users.size() - 1)
        {
            cout << ", ";
        }
    }

    cout << " }" << endl;
}

int main()
{
    cout << "========================================" << endl;
    cout << "      MINI SOCIAL NETWORK     " << endl;
    cout << "========================================" << endl;

    //1. Graph

    Graph socialNetwork;

    cout << "\n--- Creating Users ---" << endl;

    cout << "Add Alice:   "
         << socialNetwork.addUser(1, "Alice") << endl;

    cout << "Add Bob:     "
         << socialNetwork.addUser(2, "Bob") << endl;

    cout << "Add Charlie: "
         << socialNetwork.addUser(3, "Charlie") << endl;

    cout << "Add David:   "
         << socialNetwork.addUser(4, "David") << endl;

    cout << "Add Emma:    "
         << socialNetwork.addUser(5, "Emma") << endl;

    cout << "Add Frank:   "
         << socialNetwork.addUser(6, "Frank") << endl;

    // 2. Test User Management

    cout << "\n--- User Management ---" << endl;

    cout << "Does user 1 exist? ";

    if (socialNetwork.userExists(1))
        cout << "YES" << endl;
    else
        cout << "NO" << endl;

    cout << "Does user 99 exist? ";

    if (socialNetwork.userExists(99))
        cout << "YES" << endl;
    else
        cout << "NO" << endl;

    cout << "All users: ";
    printUsers(socialNetwork.getAllUsers());

    // 3.Create Friends

    cout << "\n--- Creating Friends ---" << endl;

    cout << "Alice - Bob: "
         << socialNetwork.addFriendship(1, 2) << endl;

    cout << "Alice - Charlie: "
         << socialNetwork.addFriendship(1, 3) << endl;

    cout << "Bob - David: "
         << socialNetwork.addFriendship(2, 4) << endl;

    cout << "Bob - Emma: "
         << socialNetwork.addFriendship(2, 5) << endl;

    cout << "Charlie - Emma: "
         << socialNetwork.addFriendship(3, 5) << endl;

    cout << "Charlie - Frank: "
         << socialNetwork.addFriendship(3, 6) << endl;

    cout << "Emma - Frank: "
         << socialNetwork.addFriendship(5, 6) << endl;

    // 4.Test Friendship
    cout << "\n--- Friendship Tests ---" << endl;

    cout << "Are Alice and Bob friends? ";

    if (socialNetwork.areFriends(1, 2))
        cout << "YES" << endl;
    else
        cout << "NO" << endl;


    cout << "Are Alice and David friends? ";

    if (socialNetwork.areFriends(1, 4))
        cout << "YES" << endl;
    else
        cout << "NO" << endl;


    cout << "Are Bob and Alice friends? ";

    if (socialNetwork.areFriends(2, 1))
        cout << "YES" << endl;
    else
        cout << "NO" << endl;

    // 5.Showing Friends

    cout << "\n--- Direct Friends ---" << endl;

    cout << "Alice's friends: ";
    printList(socialNetwork.getFriends(1));

    cout << "Bob's friends: ";
    printList(socialNetwork.getFriends(2));

    cout << "Charlie's friends: ";
    printList(socialNetwork.getFriends(3));

    // 6. BFS

    cout << "\n--- BFS Traversal ---" << endl;

    cout << "BFS starting from Alice (1): ";

    List<int> bfsResult =
        socialNetwork.bfsTraversal(1);

    printList(bfsResult);

    // 7. DFS

    cout << "\n--- DFS Traversal ---" << endl;

    cout << "DFS starting from Alice (1): ";

    List<int> dfsResult =
        socialNetwork.dfsTraversal(1);

    printList(dfsResult);

    // 8. MUTUAL FRIENDS

    cout << "\n--- Mutual Friends ---" << endl;

    cout << "Mutual friends of Bob (2) "
         << "and Charlie (3): ";

    List<int> mutualResult =
        socialNetwork.getMutualFriends(2, 3);

    printList(mutualResult);

    // 9. Friend Suggestions

    cout << "\n--- Friend Suggestions ---" << endl;

    cout << "Friend suggestions for Alice (1): ";

    List<int> suggestions =
        socialNetwork.suggestFriends(1);

    printList(suggestions);

    // 10. Testing Max Suggestions

    cout << "\nFriend suggestions for Alice "
         << "(maximum 1): ";

    List<int> limitedSuggestions =
        socialNetwork.suggestFriends(1, 1);

    printList(limitedSuggestions);

    // 11.Testing Invalid Users

    cout << "\n--- Invalid User Tests ---" << endl;

    cout << "BFS from nonexistent user 99: ";

    List<int> invalidBFS =
        socialNetwork.bfsTraversal(99);

    printList(invalidBFS);


    cout << "Friends of nonexistent user 99: ";

    List<int> invalidFriends =
        socialNetwork.getFriends(99);

    printList(invalidFriends);

    // 12. Testing Duplicate Friends

    cout << "\n--- Duplicate Friends Test ---" << endl;

    cout << "Trying to add Alice-Bob again: ";

    if (socialNetwork.addFriendship(1, 2))
        cout << "SUCCESS (ERROR)" << endl;
    else
        cout << "REJECTED (CORRECT)" << endl;

    // 13. Test Removal

    cout << "\n--- Removal Tests ---" << endl;

    cout << "Removing Alice-Bob: ";

    if (socialNetwork.removeFriendship(1, 2))
        cout << "SUCCESS" << endl;
    else
        cout << "FAILED" << endl;


    cout << "Are Alice and Bob friends now? ";

    if (socialNetwork.areFriends(1, 2))
        cout << "YES (ERROR)" << endl;
    else
        cout << "NO (CORRECT)" << endl;


    // Restore Alice-Bob so the network remains complete
    cout << "Restoring Alice-Bob friendship: ";

    if (socialNetwork.addFriendship(1, 2))
        cout << "SUCCESS" << endl;
    else
        cout << "FAILED" << endl;

    return 0;
}
