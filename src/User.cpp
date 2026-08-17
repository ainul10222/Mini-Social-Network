#include "User.h"
#include <cstring>


User::User() : id(-1), name(nullptr) {
    name = new char[1];
    name[0] = '\0';
}

User::User(int id, const char* name) : id(id), name(nullptr) {

    int len = static_cast<int>(strlen(name));
    this->name = new char[len + 1];
    strcpy(this->name, name);
}

User::User(const User& other) : id(-1), name(nullptr) {
    copyFrom(other);
}

User& User::operator=(const User& other) {
    if (this == &other) return *this;
    delete[] name;
    copyFrom(other);
    return *this;
}

User::~User() {
    delete[] name;
}

void User::copyFrom(const User& other) {
    id = other.id;
    int len = static_cast<int>(strlen(other.name));
    name = new char[len + 1];
    strcpy(name, other.name);
}

int User::getId() const {
    return id;
}

const char* User::getName() const {
    return name;
}

bool User::operator==(const User& other) const {
    return id == other.id;
}

bool User::operator<(const User& other) const {
    return id < other.id;
}

std::ostream& operator<<(std::ostream& out, const User& user) {
    out << "[" << user.id << "] " << user.name;
    return out;
}
