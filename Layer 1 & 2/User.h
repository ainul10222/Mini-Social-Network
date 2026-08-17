#ifndef USER_H
#define USER_H

#include <iostream>

class User {
public:
    User();

    User(int id, const char* name);
    User(const User& other);
    User& operator=(const User& other);
    ~User();

    int getId() const;
    const char* getName() const;


    bool operator==(const User& other) const;
    bool operator<(const User& other) const;
    friend std::ostream& operator<<(std::ostream& out, const User& user);

private:
    int id;
    char* name;

    void copyFrom(const User& other);
};

#endif
#define USER_H

#include <iostream>


class User {
public:
    User();
    User(int id, const char* name);
    User(const User& other);
    User& operator=(const User& other);
    ~User();

    int getId() const;
    const char* getName() const;

    bool operator==(const User& other) const;
    bool operator<(const User& other) const;
    friend std::ostream& operator<<(std::ostream& out, const User& user);

private:
    int id;
    char* name;
    void copyFrom(const User& other);
};

#endif
