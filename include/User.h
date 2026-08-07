#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

// Represents a single user (a vertex in the social graph).
// Deliberately dumb — just holds data, no graph logic here.
class User {
public:
    // Constructs a user with a unique id and display name.
    User(int id, const string& name);

    // Returns the unique identifier of this user.
    int getId() const;

    // Returns the display name of this user.
    string getName() const;

private:
    int id;
    string name;
};

#endif // USER_H
