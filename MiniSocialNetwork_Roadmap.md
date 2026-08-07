# Mini Social Network — Build Roadmap

Project: users connect as friends (undirected graph), with BFS/DFS traversal, mutual friend detection, and friend suggestions. GUI built with Qt.

This doc gives you the skeleton code for every file (declarations + comments, no logic), and the exact order to build things in. Follow the layers top to bottom — each layer only depends on the one before it, so you can test as you go instead of debugging everything at once at the end.

---

## Why this order

The biggest mistake on a project like this is opening the GUI designer first. If you do that, every bug could be a logic bug OR a Qt signal/slot wiring bug, and you won't be able to tell which. So the order is:

1. Build the **graph engine** with zero GUI involved.
2. Test it entirely from `main.cpp` printing to the console.
3. Only once BFS, DFS, mutual friends, and suggestions all work correctly on paper — wire up the GUI as a thin shell around code you already trust.

This means for weeks 1–2 you won't see a single window. That's normal and correct.

---

## Layer 1 — `User` class (data holder)

**Goal:** a simple class representing one person. No logic, just storage.

**File: `include/User.h`**
```cpp
#ifndef USER_H
#define USER_H

#include <string>

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
```

**File: `src/User.cpp`**
```cpp
#include "User.h"

User::User(int id, const string& name) {
    // TODO: assign id and name to member variables
}

int User::getId() const {
    // TODO: return the id
    return 0;
}

string User::getName() const {
    // TODO: return the name
    return "";
}
```

**What to do here:** fill in the constructor and the two getters. That's it — maybe 4 lines total. This exists so `Graph` has something to store per vertex.

---

## Layer 2 — `Graph` class, structural skeleton only

**Goal:** get the class compiling with the right data members, before writing any algorithm.

**File: `include/Graph.h`**
```cpp
#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>
#include <string>
#include "User.h"

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
```

**File: `src/Graph.cpp`**
```cpp
#include "Graph.h"
#include <queue>
#include <set>
#include <algorithm>

Graph::Graph() {
    // Nothing to initialize — containers default-construct empty.
}

bool Graph::addUser(int id, const string& name) {
    // TODO: if userExists(id), return false.
    // Otherwise insert into `users` and create an empty entry in `adjacencyList`.
    return false;
}

bool Graph::removeUser(int id) {
    // TODO: erase id from `users`.
    // Then for every OTHER user, remove id from their adjacency list.
    // Then erase id's own adjacency list entry.
    return false;
}

bool Graph::userExists(int id) const {
    // TODO: check if `users` contains this id.
    return false;
}

bool Graph::addFriendship(int id1, int id2) {
    // TODO: check both users exist and aren't already friends.
    // Push id2 into adjacencyList[id1] AND id1 into adjacencyList[id2].
    return false;
}

bool Graph::removeFriendship(int id1, int id2) {
    // TODO: erase id2 from adjacencyList[id1]'s vector,
    // AND erase id1 from adjacencyList[id2]'s vector.
    return false;
}

bool Graph::areFriends(int id1, int id2) const {
    // TODO: check if id2 appears in adjacencyList.at(id1)
    return false;
}

vector<int> Graph::bfsTraversal(int startId) const {
    // TODO: classic BFS using queue and a set (or
    // unordered_set) of visited ids. Push startId, mark visited,
    // then repeatedly pop, record, and push unvisited neighbors.
    return {};
}

vector<int> Graph::dfsTraversal(int startId) const {
    // TODO: classic DFS. Can be done recursively (helper function)
    // or iteratively with an explicit stack. Track visited
    // the same way as BFS.
    return {};
}

vector<int> Graph::getMutualFriends(int id1, int id2) const {
    // TODO: get friends of id1, get friends of id2, return the
    // intersection. set_intersection works well if both
    // friend lists are sorted first.
    return {};
}

vector<int> Graph::suggestFriends(int id, int maxSuggestions) const {
    // TODO: run BFS from id but only go 2 levels deep.
    // Collect users at exactly distance 2 (friends of friends)
    // who are not already in adjacencyList[id]. Cap at maxSuggestions.
    return {};
}

vector<int> Graph::getFriends(int id) const {
    // TODO: return adjacencyList.at(id) if it exists, else {}
    return {};
}

vector<User> Graph::getAllUsers() const {
    // TODO: collect all values from the `users` map into a vector
    return {};
}
```

**What to do here:** implement in this exact sub-order, testing each before moving to the next:
1. `addUser`, `userExists`, `getAllUsers` — get user storage working first.
2. `addFriendship`, `areFriends`, `getFriends` — get edges working.
3. `bfsTraversal` — this is the one everything else depends on.
4. `dfsTraversal` — same idea as BFS, swap queue for stack/recursion.
5. `getMutualFriends` — easy once `getFriends` works.
6. `suggestFriends` — reuses BFS logic, just capped at depth 2.
7. `removeFriendship`, `removeUser` — cleanup operations, do these last since they're the least used in a demo.

---

## Layer 3 — Console test harness (`main.cpp`, temporary version)

**Goal:** prove the graph logic works with zero GUI in the picture. You will delete/replace this main.cpp once the GUI is wired in Layer 5 — but don't skip it. This is where you actually find your bugs.

**File: `src/main.cpp` (Layer 3 version — console only)**
```cpp
// Temporary console test harness for the Graph class.
// Once Graph is verified working, this gets replaced with the
// Qt application entry point (see Layer 5).
#include <iostream>
#include "Graph.h"

int main() {
    // TODO: manually build a small test network, e.g.
    //   add 5-6 users
    //   connect several friendships forming a non-trivial shape
    //     (not just a straight line — you want branches so BFS/DFS
    //      actually have choices to make)
    //   print bfsTraversal(startId) and dfsTraversal(startId)
    //   print getMutualFriends(a, b) for a pair with a known overlap
    //   print suggestFriends(id) and manually verify it's correct
    //
    // Print everything with cout so you can eyeball correctness
    // against a hand-drawn graph on paper before trusting it.

    return 0;
}
```

**What to do here:** draw your test graph on paper first, work out BFS/DFS order and mutual friends by hand, THEN run the code and compare. Do not trust code you haven't checked against a hand worked example.

---

## Layer 4 — Qt GUI skeleton

Only start this once Layer 3 passes. At this point `Graph` is a trusted, working black box — the GUI's only job is to call its public methods and display results.

**File: `include/MainWindow.h`**
```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Main application window. Purely a shell around Graph —
// should contain no graph algorithm logic of its own.
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Reads name/id fields, calls socialGraph.addUser(...)
    void onAddUser();

    // Reads two selected user ids, calls socialGraph.addFriendship(...)
    void onAddFriendship();

    // Reads selected user id, calls socialGraph.getFriends(...),
    // displays the result in a list widget
    void onShowFriends();

    // Calls socialGraph.bfsTraversal(...), displays the order
    void onRunBFS();

    // Calls socialGraph.dfsTraversal(...), displays the order
    void onRunDFS();

    // Calls socialGraph.getMutualFriends(...), displays the result
    void onShowMutualFriends();

    // Calls socialGraph.suggestFriends(...), displays the result
    void onSuggestFriends();

private:
    Ui::MainWindow *ui;
    Graph socialGraph;

    // Repopulates any user-list dropdowns/widgets from
    // socialGraph.getAllUsers() — call this after every
    // add/remove operation so the UI stays in sync.
    void refreshUserList();
};

#endif // MAINWINDOW_H
```

**File: `src/MainWindow.cpp`**
```cpp
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    // TODO: ui->setupUi(this);
    // TODO: connect each button's clicked() signal to the
    // matching slot below (or use Qt Designer's auto-connect
    // naming convention: on_<objectName>_clicked)
}

MainWindow::~MainWindow() {
    // TODO: delete ui;
}

void MainWindow::onAddUser() {
    // TODO: read id + name from input fields
    // call socialGraph.addUser(id, name)
    // call refreshUserList()
    // clear input fields
}

void MainWindow::onAddFriendship() {
    // TODO: read two selected ids
    // call socialGraph.addFriendship(id1, id2)
    // show success/failure to the user (e.g. status bar message)
}

void MainWindow::onShowFriends() {
    // TODO: read selected id, call socialGraph.getFriends(id)
    // populate a QListWidget with the names (look up each id's
    // name — you'll want a small helper for id -> name lookup)
}

void MainWindow::onRunBFS() {
    // TODO: read selected id, call socialGraph.bfsTraversal(id)
    // display the ordered result, e.g. in a QLabel or QListWidget
}

void MainWindow::onRunDFS() {
    // TODO: same as onRunBFS but calls dfsTraversal instead
}

void MainWindow::onShowMutualFriends() {
    // TODO: read two selected ids, call getMutualFriends(id1, id2)
    // display result
}

void MainWindow::onSuggestFriends() {
    // TODO: read selected id, call suggestFriends(id)
    // display result
}

void MainWindow::refreshUserList() {
    // TODO: call socialGraph.getAllUsers()
    // clear and repopulate every dropdown/combo box that lists users
}
```

**File: `forms/MainWindow.ui`**

This is NOT hand-written — you build it visually in Qt Creator's Design view. Skeleton plan for what to drag onto the form:
- A `QLineEdit` + `QSpinBox` (or two `QLineEdit`s) for entering new user name + id, plus an "Add user" `QPushButton`.
- Two `QComboBox`es (populated from `refreshUserList()`) for picking two users, plus an "Add friendship" `QPushButton`.
- A `QComboBox` for picking a single user, with buttons: "Show friends", "Run BFS", "Run DFS", "Suggest friends".
- A second `QComboBox` pair + button for "Show mutual friends" (needs two users selected).
- A `QListWidget` or `QTextEdit` as the shared output/results panel.

**File: `resources/resources.qrc`**
```xml
<!-- TODO: register any icons/images you use (optional).
     Skeleton, empty for now:
<RCC>
  <qresource prefix="/icons">
  </qresource>
</RCC>
-->
```

---

## Layer 5 — Wiring it together (`main.cpp`, final version)

**File: `src/main.cpp` (replaces the Layer 3 version)**
```cpp
// Entry point. Launches the Qt application and shows the main window.
#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    // TODO: QApplication app(argc, argv);
    // TODO: MainWindow window;
    // TODO: window.show();
    // TODO: return app.exec();
    return 0;
}
```

**What to do here:** at this point you're just connecting wires — every real decision (how BFS works, how suggestions are computed) was already made and tested in Layer 2–3. If something looks wrong now, 90% of the time it's a signal/slot connection or a widget not being refreshed, not the graph logic.

---

## Layer 6 — Polish (do last, only if time allows)

These are extras, roughly in order of value for a course demo:

- **Input validation** — reject duplicate ids, reject friendship requests to nonexistent users, show a status bar / message box on error instead of silently failing.
- **Visual feedback** — after BFS/DFS, don't just print a list of ids; show names, and consider highlighting the traversal order with numbering.
- **Persistence** — save/load the network to a text file (`id,name,friend_id,friend_id,...` per line) so your test data survives restarting the app. Only worth doing if you have time left.
- **Remove operations in the GUI** — `removeUser`/`removeFriendship` already exist in `Graph`, wiring buttons for them is a quick win if you finish early.

---

## Build file

You'll need either a `.pro` file (qmake, what Qt Creator generates by default) or a `CMakeLists.txt` (if you're using CMake + Qt). Since you're likely using Qt Creator directly, let Qt Creator generate the `.pro` file for you when you create the project — don't hand-write it. Just make sure it lists all four `.cpp` files, all three `.h` files, and the `.ui` file.

## Quick reference — build order checklist

- [ ] `User.h` / `User.cpp` — trivial data class
- [ ] `Graph.h` — full interface, no logic yet
- [ ] `Graph.cpp` — implement addUser/userExists/getAllUsers
- [ ] `Graph.cpp` — implement addFriendship/areFriends/getFriends
- [ ] `Graph.cpp` — implement bfsTraversal
- [ ] `Graph.cpp` — implement dfsTraversal
- [ ] `Graph.cpp` — implement getMutualFriends
- [ ] `Graph.cpp` — implement suggestFriends
- [ ] `Graph.cpp` — implement removeFriendship/removeUser
- [ ] `main.cpp` (console version) — hand-verify BFS/DFS/mutual/suggest against a paper graph
- [ ] Build `MainWindow.ui` in Qt Designer
- [ ] `MainWindow.h` / `MainWindow.cpp` — wire buttons to Graph calls
- [ ] `main.cpp` (Qt version) — replace console harness
- [ ] Polish (optional)
