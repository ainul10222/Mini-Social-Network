#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCompleter>
#include <QLabel>
#include <QPushButton>
#include <QStringListModel>
#include <vector>

#include "DataStore.h"
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
    void onContinue();
    void onOpenLogin();
    void onOpenRegister();
    void onOpenDelete();
    void onBackToAccess();
    void onExit();
    void onLogin();
    void onRegister();
    void onDeleteAccount();
    void onProfileFriendAction();
    void onShowFriends();
    void onSearchUser();
    void onRunBFS();
    void onRunDFS();
    void onShowMutualFriends();
    void onSuggestFriends();
    void onLogout();

private:
    enum class SearchMode { None, SearchUser, MutualFriends };

    Ui::MainWindow *ui;
    Graph socialGraph;
    DataStore dataStore;
    vector<Account> accounts;
    int nextUserId;
    int currentUserId;
    int viewedUserId;
    SearchMode searchMode;
    QCompleter* userCompleter;
    QStringListModel* userSuggestionModel;
    QLabel* profileViewLabel;
    QPushButton* profileFriendButton;
    QWidget* headerWidget;
    QLabel* headerAvatarLabel;
    QLabel* headerAccountLabel;

    int findUserId(const QString& searchTerm) const;
    const Account* findAccount(const QString& username) const;
    const Account* findAccountByUserId(int userId) const;
    void showStatus(const QString& message);
    void showIds(const QString& label, const List<int>& ids);
    void openSearchMode(SearchMode mode);
    void closeSearchMode();
    void refreshUserSuggestions();
    void showUserProfile(int userId);
};

#endif // MAINWINDOW_H
