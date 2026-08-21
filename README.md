# Mini Social Network

A beginner-friendly social network simulation built with **C++17**. The project models users and friendships as an undirected graph and provides both a terminal-based interface and an optional desktop GUI powered by **Qt 5**.

## Project Status

The core social-network engine is implemented and can be run from the terminal. The Qt5 interface provides screens for registration, login, account deletion, profiles, friendships, searches, graph traversal, mutual friends, and friend suggestions.

## Project Structure

```text
Mini-Social-Network/
├── main.cpp                         # Terminal application entry point
├── CMakeLists.txt                   # CMake build configuration
├── README.md                        # Project documentation
├── MiniSocialNetwork_Roadmap.md     # Development roadmap
├── include/
│   ├── ArrayQueue.h                 # Dynamic queue implementation
│   ├── ArrayStack.h                 # Dynamic stack implementation
│   ├── DataStore.h                  # Account and persistence declarations
│   ├── Graph.h                      # Social graph declarations
│   ├── LinkedList.h                 # Linked-list implementation
│   ├── List.h                       # Dynamic list implementation
│   └── User.h                       # User model
├── src/
│   ├── DataStore.cpp                # Save/load social data
│   ├── Graph.cpp                    # Graph operations and algorithms
│   └── User.cpp                     # User implementation
├── GUI/
│   ├── forms/MainWindow.ui          # Qt Designer interface
│   ├── include/MainWindow.h         # Qt window declaration
│   ├── resources/resources.qrc      # GUI resources
│   └── src/                         # Qt application sources
└── Persistence/
	├── social_network.txt           # Terminal/GUI text persistence file
	└── social_network.json          # Project data file
```

## Features

### Account Management

- Register an account with a username, password, display name, and unique UID
- Log in and log out
- Delete an account and all of its friendships
- Persist account and social-network data between runs

### Friendship Graph

- Add and remove friendships
- Prevent duplicate friendships and self-friendships
- Display a user's friends
- Search users by display name or UID
- Find mutual friends between two users
- Suggest friends through friends-of-friends

### Graph Algorithms

- **Breadth-First Search (BFS)** from the current user
- **Depth-First Search (DFS)** from the current user
- Mutual-friend detection
- Friend suggestions based on graph connections

### Two Interfaces

- **Terminal mode:** interactive text menus in the console
- **Qt5 GUI mode:** desktop interface with forms, profile views, search, and social actions

## Technology Stack

- C++17
- CMake 3.10 or newer
- Qt 5 Widgets for the GUI
- Custom dynamic arrays, lists, stacks, queues, and linked lists
- Text-file persistence in `Persistence/social_network.txt`

## Quick Start: Terminal Version

### Prerequisites

Install:

- A C++17 compiler
- CMake
- Ninja or Make

### Windows with MSYS2 UCRT64

Open the **MSYS2 UCRT64** terminal and install the compiler and build tools:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain \
	mingw-w64-ucrt-x86_64-cmake \
	mingw-w64-ucrt-x86_64-ninja
```

Move to the project directory. Adjust the path if your project is stored elsewhere:

```bash
cd /d/Projects/C++/Mini-Social-Network
```

Configure and build the terminal version:

```bash
cmake -S . -B build-msys2 -G "MinGW Makefiles" -DBUILD_GUI=OFF
cmake --build build-msys2
```

Run it from the project directory so the persistence path works correctly:

```bash
./build-msys2/MiniSocialNetwork.exe
```

If you configured the project inside the `build` directory instead, run:

```bash
cd build
./MiniSocialNetwork.exe
```

### Windows with Visual Studio or PowerShell

From the project directory:

```powershell
cmake -S . -B build -DBUILD_GUI=OFF
cmake --build build
.\build\MiniSocialNetwork.exe
```

## Quick Start: Qt5 GUI Version

### Prerequisites

Install Qt5 with the Widgets module. With MSYS2 UCRT64, the package name is typically:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-qt5-base
```

Configure and build the GUI version:

```bash
cd /d/Projects/C++/Mini-Social-Network
cmake -S . -B build-gui-msys2 -G "MinGW Makefiles" -DBUILD_GUI=ON
cmake --build build-gui-msys2
```

Run the GUI:

```bash
./build-gui-msys2/MiniSocialNetwork.exe
```

The GUI build uses `Qt5::Widgets`, as configured in `CMakeLists.txt`.

## Terminal Menu

After launching the terminal version, the application provides:

```text
1. Register
2. Login
3. Delete account
0. Exit
```

After logging in:

```text
1. Add friend
2. Remove friend
3. Show my friends
4. Find users
5. Run BFS from me
6. Run DFS from me
7. Show mutual friends
8. Suggest friends
9. Logout
0. Exit
```

## Example Workflow

```text
1. Register
   Username: alice
   Display name: Alice
   UID: alice01

2. Register another user
3. Log in as Alice
4. Add the other user as a friend
5. Show friends or run BFS/DFS
6. Search by display name or UID
```

Account and friendship changes are saved to:

```text
Persistence/social_network.txt
```

## Technical Architecture

### User Model

`User` stores a numeric ID and display name. It represents a vertex in the social graph.

### Graph Model

`Graph` stores users and friendships using dynamically allocated arrays and linked lists. Friendships are undirected, so adding a friendship updates both users' friend lists.

### Data Structures

- `List<T>` provides a dynamic array-style list
- `LinkedList<T>` stores linked nodes
- `ArrayQueue<T>` supports breadth-first traversal
- `ArrayStack<T>` supports depth-first traversal

### Persistence

`DataStore` serializes accounts, users, and friendships into a text file and restores them when the application starts.

### GUI Architecture

The Qt5 window acts as a user interface layer around the existing `Graph` and `DataStore` classes. The graph algorithms remain in the core C++ source files and can be used without Qt.

## Learning Outcomes

- Object-oriented programming with C++
- Graph representation and traversal algorithms
- BFS and DFS implementation
- Dynamic memory management
- Custom data-structure implementation
- File input/output and persistence
- CMake-based project organization
- Qt5 GUI development
- Separating application logic from the user interface

## Contributing

1. Create a feature branch.
2. Keep core graph logic independent from GUI code.
3. Build both terminal and GUI modes when changing shared classes.
4. Test account, friendship, persistence, BFS, DFS, and suggestion behavior.
5. Open a pull request with a clear description of the change.

## License

This project is licensed under the [Apache License 2.0](LICENSE).
