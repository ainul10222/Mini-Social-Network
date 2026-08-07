#include "MainWindow.h"
#include "ui_MainWindow.h"
using namespace std;

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
