#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Graph.h"
using namespace std;

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
