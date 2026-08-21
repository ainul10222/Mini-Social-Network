# Mini Social Network: Beginner's Project Guide

This document explains how the current Mini Social Network project works, how its files are connected, and what each important function does.

The application is a Qt 5 desktop GUI backed by a C++ social-network engine. Users are stored in a graph, friendships are graph edges, and account/network data is saved in `Persistence/social_network.txt`.

## 1. The Big Picture

The project has four layers:

```text
Qt application startup
        |
        v
MainWindow (buttons, forms, screen changes, messages)
        |
        +--> DataStore (load and save accounts/network)
        |
        +--> Graph (users, friendships, BFS, DFS, suggestions)
                    |
                    +--> User
                    +--> LinkedList<int>
                    +--> List<int>
                    +--> ArrayQueue<int> / ArrayStack<int>
```

A normal user action follows this pattern:

1. The user clicks a button in the Qt window.
2. Qt emits the button's `clicked` signal.
3. A `MainWindow` slot receives the signal.
4. The slot reads values from the form and validates them.
5. The slot calls `Graph` to change or inspect the social network.
6. The slot calls `DataStore::save` when data changed.
7. The slot displays a result in the GUI.

The GUI does not use the old terminal menu. `GUI/src/main.cpp` is now the only application entry point.

## 2. Starting the Application

### `GUI/src/main.cpp`

This file is intentionally small because startup belongs here, while application behavior belongs in `MainWindow`.

```cpp
QApplication application(argc, argv);
MainWindow window;
window.show();
return application.exec();
```

- `QApplication application(argc, argv)` creates the Qt application object. It prepares the event system and reads standard command-line arguments.
- `MainWindow window` constructs the main window. Its constructor loads data, builds additional widgets, applies styles, and connects signals to slots.
- `window.show()` makes the window visible.
- `application.exec()` starts Qt's event loop. The program waits for events such as clicks, text changes, and window closing until the application exits.

The event loop is why the GUI can remain responsive without a manual `while` loop.

## 3. Build Configuration

### `CMakeLists.txt`

CMake describes how to build the executable.

- `cmake_minimum_required` sets the minimum supported CMake version.
- `project(MiniSocialNetwork LANGUAGES CXX)` creates a C++ project.
- `CMAKE_CXX_STANDARD 17` requires C++17.
- The two `include_directories` entries allow source files to include files from `include/` and `GUI/include/`.
- `SRC` lists the ordinary C++ source files. The GUI `main.cpp`, `User.cpp`, `Graph.cpp`, and `DataStore.cpp` are compiled directly.
- `CMAKE_AUTOMOC` enables Qt's automatic processing of classes containing `Q_OBJECT`.
- `find_package(Qt5 COMPONENTS Widgets REQUIRED)` locates Qt Widgets. Configuration fails if Qt 5 Widgets is unavailable.
- `qt5_wrap_ui` converts `GUI/forms/MainWindow.ui` into generated C++ code. The generated `ui_MainWindow.h` contains widget declarations and setup code.
- `qt5_add_resources` converts `GUI/resources/resources.qrc` into C++ resource code. This makes the logo and background available through paths such as `:/branding/logo.jpg`.
- `add_executable` creates the `MiniSocialNetwork` executable.
- `target_include_directories` lets the compiler find generated UI headers.
- `target_link_libraries` connects the program to `Qt5::Widgets`.

Typical build commands are:

```powershell
cmake -S . -B build-gui -G Ninja
cmake --build build-gui
.\build-gui\MiniSocialNetwork.exe
```

## 4. The GUI Files

### `GUI/forms/MainWindow.ui`

This is the Qt Designer form. It describes the widgets and their names, but not the application's business logic.

Important widgets include:

- `pages`: a `QStackedWidget` containing the welcome, access, login, registration, delete-account, and social pages.
- `continueButton`: leaves the welcome page.
- `accessLoginButton`, `accessRegisterButton`, `accessDeleteButton`, `exitButton`: access-page actions.
- Login fields: `loginUsernameEdit`, `loginPasswordEdit`, and `loginButton`.
- Registration fields: `registerUsernameEdit`, `registerPasswordEdit`, `displayNameEdit`, `uidEdit`, and `registerButton`.
- Delete fields: `deleteUsernameEdit`, `deletePasswordEdit`, and `deleteAccountButton`.
- Dashboard actions: `showFriendsButton`, `searchButton`, `bfsButton`, `dfsButton`, `mutualButton`, `suggestButton`, and `logoutButton`.
- `friendTargetEdit`: the temporary search input used for user search and mutual-friend lookup.
- `outputEdit`: read-only text area used to show results.
- `authStatusLabel` and `dashboardStatusLabel`: status message labels.

The `.ui` file is converted into `ui_MainWindow.h` during the build. That generated header should not be edited manually.

### `GUI/resources/resources.qrc`

The resource file gives image files application resource names:

- `resources/mini_social_media_logo.jpg` becomes `:/branding/logo.jpg`.
- `resources/backgroundpicture.jpg` becomes `:/branding/background.jpg`.

Qt embeds these resources into the executable through generated C++ code.

## 5. The Main Window Controller

### `GUI/include/MainWindow.h`

`MainWindow` inherits from `QMainWindow`, so it is the controller for the window and its user interactions.

The `Q_OBJECT` macro enables Qt's signal-and-slot meta-object system.

#### Main window fields

- `ui`: pointer to the generated UI object. It gives the implementation access to named widgets.
- `socialGraph`: stores users and friendships.
- `dataStore`: reads and writes `Persistence/social_network.txt`.
- `accounts`: standard C++ vector containing usernames, passwords, UIDs, and graph user IDs.
- `nextUserId`: ID assigned to the next new account.
- `currentUserId`: logged-in user's graph ID, or `-1` when nobody is logged in.
- `viewedUserId`: profile currently being viewed, or `-1` when no profile is selected.
- `searchMode`: tells the shared search input whether it is being used for user search or mutual-friend search.
- `userCompleter`: Qt popup completion controller.
- `userSuggestionModel`: list of names shown by the completer.
- `profileViewLabel` and `profileFriendButton`: widgets created at runtime for a searched profile.
- `headerWidget`, `headerAvatarLabel`, and `headerAccountLabel`: widgets created for the logged-in header.

#### Navigation slots

- `onContinue`: changes the stacked widget from the welcome page to the access page.
- `onOpenLogin`: opens the login page.
- `onOpenRegister`: opens the registration page.
- `onOpenDelete`: opens the delete-account page.
- `onBackToAccess`: returns from login, registration, or deletion to the access page.
- `onExit`: calls `close()` on the main window, which ends the application event loop.

#### Constructor and destructor

`MainWindow::MainWindow` performs the initial setup:

1. Calls `ui->setupUi(this)` to create the widgets described by the `.ui` file.
2. Calls `dataStore.load` to restore accounts, users, friendships, and the next ID.
3. Creates the custom header, profile widgets, and search completer.
4. Hides widgets that only make sense after login or during a search.
5. Applies the Qt stylesheet.
6. Connects each button or input signal to its slot.

`MainWindow::~MainWindow` saves the current state and deletes `ui`. Saving here means changes are written again when the window closes, even if the action that changed them already saved.

#### Account helper functions

- `findAccount(username)`: scans `accounts` and returns a pointer to the account with that username. It returns `nullptr` when there is no match.
- `findAccountByUserId(userId)`: scans `accounts` for the account connected to a graph user ID.
- `findUserId(searchTerm)`: first searches graph display names, then account UIDs. It returns the matching graph ID or `-1`.

The graph stores display names and IDs. The account vector stores login information and UIDs. These helper functions bridge the two collections.

#### Output helper functions

- `showStatus(message)`: puts a message in both status labels and appends it to the dashboard output text area.
- `showIds(label, ids)`: converts graph IDs into display names and appends a readable list to `outputEdit`. An empty list displays `None`.

#### Search helper functions

- `openSearchMode(mode)`: records the requested mode, clears and shows the shared input, focuses it, sets a placeholder, and refreshes suggestions.
- `closeSearchMode`: resets the mode, clears the input, and hides it.
- `refreshUserSuggestions`: builds a list of users whose display name or UID contains the current search text, ignoring case, and gives it to the Qt completer.
- `showUserProfile(userId)`: looks up a graph user, finds its UID, updates the profile label, chooses `Add Friend` or `Remove Friend`, and displays the profile widgets.

#### Login and registration slots

`onLogin`:

1. Reads and trims the username and reads the password.
2. Finds the matching account.
3. Compares the entered password with the stored password.
4. On failure, shows an error and stops.
5. On success, sets `currentUserId`, updates the header and profile card, switches to the social page, and displays a success message.

`onRegister`:

1. Reads the four registration fields.
2. Rejects empty fields.
3. Rejects duplicate usernames.
4. Rejects duplicate UIDs.
5. Increments `nextUserId`.
6. Adds an `Account` to the account vector.
7. Adds a `User` with the same ID to `socialGraph`.
8. Saves the new state.
9. Shows a success message and opens the login page.

A username is used for authentication, a UID is a separate user-facing identifier, and the numeric `userId` connects the account to the graph.

`onDeleteAccount`:

1. Reads the username and password.
2. Searches for an account matching both values.
3. On failure, leaves all data unchanged and reports an error.
4. On success, calls `Graph::removeUser`, which removes the user and all connected friendships.
5. Removes the account from the vector.
6. Saves the result and displays a confirmation.

#### Friendship and graph-action slots

`onProfileFriendAction` toggles the relationship with the viewed profile. It uses `Graph::areFriends` to decide whether to remove or add the edge, saves the graph, updates the button text, and reports the result.

`onShowFriends` calls `Graph::getFriends` for the logged-in user and displays the names.

`onSearchUser` uses two clicks/steps. The first click opens search mode. The second click reads the input, finds a user by display name or UID, shows that profile, and closes the search mode.

`onRunBFS` calls `Graph::bfsTraversal(currentUserId)` and displays the returned order.

`onRunDFS` calls `Graph::dfsTraversal(currentUserId)` and displays the returned order.

`onShowMutualFriends` first opens mutual-search mode. On the next activation, it finds the other user and calls `Graph::getMutualFriends`.

`onSuggestFriends` calls `Graph::suggestFriends(currentUserId)` and displays the result.

`onLogout` resets the current user and search state, hides logged-in widgets, returns to the access page, clears login fields, and reports the logout.

## 6. Account and Persistence Data

### `include/DataStore.h`

`Account` is a simple structure:

- `username`: login name.
- `password`: stored password.
- `uid`: user-facing unique identifier.
- `userId`: numeric ID shared with `Graph`.

`DataStore` owns one field, `fileName`, which defaults to `Persistence/social_network.txt`.

### `src/DataStore.cpp`

`DataStore::DataStore(fileName)` stores the path to use for persistence.

`DataStore::load(socialGraph, accounts, nextUserId)`:

1. Opens the text file. If it does not exist, it returns `true` and leaves the data empty.
2. Tracks which section is being read: `ACCOUNTS`, `USERS`, or `FRIENDSHIPS`.
3. Reads account strings with `std::quoted`, so spaces can be preserved.
4. Adds each loaded account to the vector.
5. Adds each loaded user to the graph.
6. Adds each loaded friendship to the graph.
7. Finds the highest numeric user ID and sets `nextUserId` to one greater.

`DataStore::save(socialGraph, accounts)`:

1. Opens the file for writing.
2. Writes the `ACCOUNTS` section.
3. Writes the `USERS` section using `Graph::getAllUsers`.
4. Writes the `FRIENDSHIPS` section.
5. Writes each undirected friendship only once by writing an edge when `userId < friendId`.
6. Returns `true` if the file was opened and written through the function.

The text file has this general shape:

```text
ACCOUNTS
"username" "password" "uid" 1
USERS
1 "Display Name"
FRIENDSHIPS
1 2
```

The active persistence format is the text file `Persistence/social_network.txt`.

## 7. The `User` Class

### `include/User.h` and `src/User.cpp`

`User` represents one vertex in the social graph.

Fields:

- `id`: numeric graph ID.
- `name`: dynamically allocated C-style string.

Functions:

- `User()`: creates an empty user with ID `-1` and an empty allocated name.
- `User(id, name)`: allocates enough memory for the provided name and copies it.
- `User(const User&)`: copy constructor; creates an independent copy of the name.
- `operator=`: handles assignment, releasing the old name before copying the new one.
- `~User()`: releases the allocated name with `delete[]`.
- `copyFrom`: private helper used by the copy constructor and assignment operator.
- `getId`: returns the numeric ID.
- `getName`: returns the stored name pointer.
- `operator==`: considers two users equal when their IDs match.
- `operator<`: compares users by ID.
- `operator<<`: formats a user as `[id] name` for stream output.

The copy operations matter because `Graph` moves and copies `User` objects when it resizes or shifts its arrays. A shallow pointer copy would cause double deletion, so `User` performs deep copies.

## 8. The `Graph` Class

### Internal representation

`Graph` uses parallel arrays:

```text
users[index]  <-->  friends[index]
```

For a particular index, `users[index]` is the user and `friends[index]` is that user's linked list of friend IDs.

The arrays are kept sorted by user ID. This allows `findIndex` and `findInsertPos` to use binary search.

Fields:

- `users`: dynamic array of `User` objects.
- `friends`: dynamic array of `LinkedList<int>` adjacency lists.
- `count`: number of active users.
- `capacity`: allocated array size.

### Graph lifetime and storage functions

- `Graph(initialCapacity)`: allocates the two arrays, using at least capacity 1.
- `Graph(const Graph&)`: creates a deep copy of another graph.
- `operator=`: destroys the current arrays and deep-copies another graph.
- `~Graph`: releases both arrays.
- `destroy`: deletes arrays and resets their pointers.
- `copyFrom`: allocates arrays and copies every active user and adjacency list.
- `resize`: allocates larger arrays, copies active entries, deletes old arrays, and updates capacity.

### Search and user functions

`findIndex(id)` performs binary search over the sorted user array. It returns the array index or `-1`.

`findInsertPos(id)` finds the position where a new ID belongs so the array remains sorted.

`findUser(id)` returns a pointer to the matching user or `nullptr`.

`userExists(id)` is a convenience wrapper around `findIndex`.

`addUser(id, name)`:

1. Rejects duplicate IDs.
2. Resizes if the arrays are full.
3. Finds the sorted insertion position.
4. Shifts later users and friend lists one position to the right.
5. Constructs the new user and an empty adjacency list.
6. Increases `count`.

`removeUser(id)`:

1. Finds the user.
2. Removes that ID from every other user's friendship list.
3. Shifts later users and adjacency lists left.
4. Decreases `count`.

This removes both the vertex and every edge connected to it.

`getAllUsers()` returns a new `List<User>` containing copies of all active users.

### Friendship functions

`addFriendship(id1, id2)` rejects self-friendships, missing users, and duplicate edges. For a valid relationship it adds `id2` to `id1`'s list and `id1` to `id2`'s list. This is how an undirected edge is represented.

`removeFriendship(id1, id2)` removes both directions and returns whether both removals succeeded.

`areFriends(id1, id2)` checks whether both users exist and whether `id2` appears in `id1`'s adjacency list.

`getFriends(id)` converts the selected user's linked list into a `List<int>`. A missing user produces an empty list.

### Traversal and analysis functions

`bfsTraversal(startId)` performs breadth-first search:

1. Rejects a missing start user with an empty result.
2. Creates a boolean visited array and an `ArrayQueue<int>`.
3. Marks the start user visited and enqueues its ID.
4. Repeatedly dequeues the next ID, adds it to the result, and enqueues each unvisited neighbor.
5. Returns the visit order.

BFS explores by distance: the starting user first, then direct friends, then friends two steps away, and so on.

`dfsTraversal(startId)` performs depth-first search:

1. Rejects a missing start user.
2. Creates a visited array and an `ArrayStack<int>`.
3. Pushes the start ID.
4. Pops an ID, skips it if already visited, otherwise marks and records it.
5. Pushes its unvisited neighbors.
6. Continues until the stack is empty.

DFS follows one available path as far as possible before backing up. Because friendships are inserted at the front of linked lists, the exact traversal order depends on insertion order.

`getMutualFriends(id1, id2)` gets both adjacency lists and adds each ID that appears in both lists. The result contains users directly connected to both requested users.

`suggestFriends(id, maxSuggestions)`:

1. Gets the user's direct friends.
2. Looks at every direct friend's friends.
3. Skips the current user.
4. Skips people already directly connected.
5. Skips duplicate suggestions.
6. Stops at `maxSuggestions`.

This is a simple friends-of-friends recommendation algorithm. It does not calculate a ranking or friendship strength.

## 9. Custom Container Classes

These templates are header-only because the compiler needs to see their complete implementation whenever a particular type is used.

### `List<T>`

`List<T>` is a dynamic array similar to a simplified `std::vector`.

- Constructor: allocates an array with an initial capacity of at least 1.
- Copy constructor: deep-copies the elements and capacity.
- Assignment operator: handles self-assignment, deletes the old array, then copies.
- Destructor: deletes the array.
- `add`: appends an item and doubles capacity when full.
- `remove`: finds and removes the first equal item, shifting later elements left.
- `contains`: checks whether an item exists.
- `indexOf`: returns the first matching index or `-1`.
- `size`: returns the number of active elements.
- `isEmpty`: checks whether the count is zero.
- `operator[]`: gives access to an element by index.
- `resize`: allocates a larger array and copies active elements.
- `copyFrom`: copies another list's storage and elements.
- `operator<<`: prints the list as `{ item1, item2 }`.

`List<T>` assumes that `T` supports equality because `remove`, `contains`, and `indexOf` use `operator==`.

### `LinkedList<T>`

`LinkedList<T>` stores nodes connected by pointers. It is used for each user's adjacency list.

- `Node`: stores one value and a pointer to the next node.
- Constructor: starts empty.
- Copy constructor and `copyFrom`: create new nodes rather than sharing nodes.
- Assignment operator: clears old nodes, then copies.
- Destructor and `clear`: delete every node.
- `insertFront`: adds a value at the beginning in constant time.
- `remove`: finds and deletes the first matching node.
- `contains`: searches nodes for a value.
- `size` and `isEmpty`: report list state.
- `toList`: copies linked values into a `List<T>` for indexed iteration.
- `operator<<`: prints a linked list as `( value -> value )`.

### `ArrayQueue<T>`

`ArrayQueue<T>` is a circular dynamic queue used by BFS.

- `enqueue`: adds at the rear, wrapping around when necessary.
- `dequeue`: removes and returns the front item.
- `front`: returns the front item without removing it.
- `isEmpty` and `size`: report queue state.
- `resize`: copies elements in logical queue order into a larger array.
- Copy constructor, assignment operator, destructor, and `copyFrom`: manage independent dynamic storage.

The queue uses `frontIdx` and `count`, so the physical array position of the rear is `(frontIdx + count) % capacity`.

### `ArrayStack<T>`

`ArrayStack<T>` is a dynamic last-in, first-out stack used by DFS.

- `push`: places a value at the top, growing the array if needed.
- `pop`: returns and removes the top value.
- `peek`: returns the top value without removing it.
- `isEmpty` and `size`: report stack state.
- `resize`: allocates a larger array and copies stack elements.
- Copy constructor, assignment operator, destructor, and `copyFrom`: manage independent storage.

Both `pop` and `peek` expect the caller to check that the structure is not empty first. The graph algorithms do this with `while (!q.isEmpty())` and `while (!s.isEmpty())`.

## 10. A Complete User Flow

### Registering

```text
Register form
    -> MainWindow::onRegister
    -> validate username/password/display name/UID
    -> accounts.push_back(Account)
    -> Graph::addUser
    -> DataStore::save
```

The account and graph user are created with the same numeric ID.

### Logging in

```text
Login form
    -> MainWindow::onLogin
    -> findAccount
    -> compare password
    -> set currentUserId
    -> switch pages to socialPage
```

No new graph user is created during login. Login only selects an existing account.

### Adding a friend

A search locates another user's numeric ID. The profile action then calls:

```text
Graph::areFriends
    -> Graph::removeFriendship OR Graph::addFriendship
    -> DataStore::save
    -> update profile button and status text
```

The graph stores both directions so either user can see the relationship.

### Running BFS

```text
BFS button
    -> MainWindow::onRunBFS
    -> Graph::bfsTraversal(currentUserId)
    -> List<int> of IDs
    -> MainWindow::showIds
    -> Graph::findUser for each ID
    -> outputEdit
```

The algorithm returns IDs because IDs are the graph's stable internal references. The GUI converts those IDs into names for readability.

### Closing the application

```text
window close
    -> MainWindow::~MainWindow
    -> DataStore::save
    -> Graph and account objects are destroyed
    -> QApplication::exec returns
```

## 11. Memory Management Concepts Used

This project intentionally demonstrates manual memory management.

- `new[]` allocates arrays for users, linked lists, list elements, queue elements, stack elements, and user names.
- `delete[]` releases arrays and C-style strings.
- `delete` releases individual linked-list nodes.
- Copy constructors and assignment operators prevent two objects from owning the same allocated memory.
- `nullptr` represents a missing pointer, such as a user that could not be found.

The important ownership rules are:

- A `User` owns its dynamically allocated `name`.
- A `Graph` owns its `users` and `friends` arrays.
- A `LinkedList` owns its nodes.
- A `List`, `ArrayQueue`, and `ArrayStack` each own their backing array.
- `MainWindow` owns the generated `ui` pointer and gives runtime-created Qt widgets a Qt parent, allowing Qt to clean those widgets up.

## 12. Things to Keep in Mind

- Passwords are stored as plain text in `Persistence/social_network.txt`. This is acceptable for a learning project but not for a real service.
- The application currently uses the text persistence file, not the JSON file.
- The graph uses a numeric ID internally, while usernames and UIDs are account-level lookup values.
- BFS and DFS return traversal IDs, not names. The GUI performs the name conversion.
- The suggestion algorithm only searches friends-of-friends and returns at most five results by default.
- The custom containers do not perform bounds checks for `operator[]`, `pop`, `peek`, or `dequeue`; callers must use valid indexes and check emptiness where required.
- `using namespace std` appears in several headers. It works here, but avoiding it in headers is generally better practice because headers affect every file that includes them.

## 13. Quick File Reference

| File | Responsibility |
| --- | --- |
| `GUI/src/main.cpp` | Starts Qt and opens the main window |
| `GUI/include/MainWindow.h` | Declares GUI state, slots, and helpers |
| `GUI/src/MainWindow.cpp` | Implements GUI behavior and connects actions to the model |
| `GUI/forms/MainWindow.ui` | Defines the Qt widgets and pages |
| `GUI/resources/resources.qrc` | Embeds GUI images as Qt resources |
| `include/User.h` | Declares the user model |
| `src/User.cpp` | Implements user storage, copying, comparison, and output |
| `include/Graph.h` | Declares social graph operations |
| `src/Graph.cpp` | Implements users, friendships, traversals, and suggestions |
| `include/DataStore.h` | Declares accounts and persistence API |
| `src/DataStore.cpp` | Reads and writes the text data file |
| `include/List.h` | Dynamic array template |
| `include/LinkedList.h` | Linked-list template |
| `include/ArrayQueue.h` | Circular queue template for BFS |
| `include/ArrayStack.h` | Stack template for DFS |
| `CMakeLists.txt` | Configures the Qt/C++ build |
| `Persistence/social_network.txt` | Active saved application data |
| `Persistence/social_network.txt` | Active saved application data |

## 14. Recommended Reading Order

For learning the project, read the files in this order:

1. `GUI/src/main.cpp`
2. `GUI/include/MainWindow.h`
3. `GUI/src/MainWindow.cpp`
4. `include/DataStore.h` and `src/DataStore.cpp`
5. `include/Graph.h` and `src/Graph.cpp`
6. `include/User.h` and `src/User.cpp`
7. `include/List.h`
8. `include/LinkedList.h`
9. `include/ArrayQueue.h`
10. `include/ArrayStack.h`
11. `GUI/forms/MainWindow.ui`
12. `CMakeLists.txt`

Start with the GUI flow, then follow one action such as registration or BFS downward into the model. That makes the relationships between the files much easier to see than reading every utility class in isolation.
