#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QHBoxLayout>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), nextUserId(1), currentUserId(-1), viewedUserId(-1), searchMode(SearchMode::None), userCompleter(nullptr), userSuggestionModel(nullptr), profileViewLabel(nullptr), profileFriendButton(nullptr), headerWidget(nullptr), headerAvatarLabel(nullptr), headerAccountLabel(nullptr) {
    ui->setupUi(this);
    dataStore.load(socialGraph, accounts, nextUserId);
    ui->logoLabel->show();
    auto* header = new QWidget(ui->centralwidget);
    headerWidget = header;
    header->setObjectName("header");
    auto* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(18, 5, 18, 5);
    headerLayout->setSpacing(8);
    auto* headerLogo = new QLabel(header);
    QPixmap logo(":/branding/logo.jpg");
    headerLogo->setPixmap(logo.scaled(155, 42, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    headerLogo->setFixedSize(160, 44);
    headerLogo->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    headerAvatarLabel = new QLabel("A", header);
    headerAvatarLabel->setObjectName("headerAvatarLabel");
    headerAvatarLabel->setAlignment(Qt::AlignCenter);
    headerAvatarLabel->setFixedSize(34, 34);
    headerAccountLabel = new QLabel("Guest", header);
    headerAccountLabel->setObjectName("headerAccountLabel");
    headerLayout->addWidget(headerLogo);
    headerLayout->addStretch();
    headerLayout->addWidget(headerAvatarLabel);
    headerLayout->addWidget(headerAccountLabel);
    header->setFixedHeight(54);
    header->hide();
    ui->rootLayout->insertWidget(0, header);
    ui->friendTargetEdit->hide();
    ui->addFriendButton->hide();
    ui->removeFriendButton->hide();
    profileViewLabel = new QLabel(ui->resultPanel);
    profileFriendButton = new QPushButton("Add Friend", ui->resultPanel);
    profileFriendButton->setObjectName("profileFriendButton");
    profileViewLabel->hide();
    profileFriendButton->hide();
    ui->resultLayout->insertWidget(1, profileViewLabel);
    ui->resultLayout->insertWidget(2, profileFriendButton);
    userCompleter = new QCompleter(this);
    userSuggestionModel = new QStringListModel(userCompleter);
    userCompleter->setModel(userSuggestionModel);
    userCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    userCompleter->setCompletionMode(QCompleter::PopupCompletion);
    ui->friendTargetEdit->setCompleter(userCompleter);

    setStyleSheet("QMainWindow { background: #f8f8f5; }"
                  "QWidget#centralwidget { border-image: url(:/branding/background.jpg) 0 0 0 0 stretch stretch; background: transparent; }"
                  "QWidget#header { background: rgba(255, 255, 255, 160); border: 1px solid #e5e5e5; }"
                  "QLabel#headerAvatarLabel { background: #a9a9a9; color: #ffffff; border-radius: 17px; font-size: 15px; font-weight: 700; }"
                  "QLabel#headerAccountLabel { color: #3f3f3f; font-size: 11px; font-weight: 600; }"
                  "QStackedWidget#pages, QWidget#socialPage, QWidget#dashboardContent { background: transparent; }"
                  "QLabel#titleLabel, QLabel#welcomeTitle, QLabel#accessTitle, QLabel#loginTitle, QLabel#registerTitle, QLabel#deleteTitle { color: #252525; font-size: 28px; font-weight: 500; }"
                  "QLabel#welcomeSubtitle, QLabel#accessSubtitle, QLabel#welcomeHint { color: #666666; font-size: 17px; }"
                  "QPushButton { min-height: 44px; border-radius: 10px; padding: 0 22px; font-size: 14px; font-weight: 700; background: rgba(255, 255, 255, 160); border: 2px solid #2b9bb2; color: #287e91; }"
                  "QPushButton:hover { background: #eaf8fa; }"
                  "QPushButton#accessDeleteButton { border-color: #c76b66; color: #b65e59; }"
                  "QPushButton#accessDeleteButton:hover { background: #fff0ef; }"
                  "QPushButton#exitButton { border-color: #bcb7ae; color: #8f8a82; }"
                  "QFrame#sidebar { background: rgba(13, 80, 103, 160); border: none; }"
                  "QLabel#sidebarLogo { color: #ffffff; font-size: 24px; font-weight: 700; padding: 14px 8px; }"
                  "QFrame#profileCard { background: rgba(40, 107, 128, 160); border: none; border-radius: 10px; padding: 10px; }"
                  "QLabel#profileNameLabel { color: #ffffff; font-size: 15px; font-weight: 700; }"
                  "QLabel#profileStatusLabel { color: #d6edf0; font-size: 12px; }"
                  "QPushButton#sidebar QPushButton, QFrame#sidebar QPushButton { text-align: left; border: none; border-radius: 8px; color: #eefcff; background: transparent; min-height: 42px; font-size: 15px; font-weight: 400; }"
                  "QFrame#sidebar QPushButton:hover { background: #286b80; }"
                  "QLabel#dashboardTitle { color: #0f1828; font-size: 30px; font-weight: 600; padding-bottom: 12px; }"
                  "QFrame#resultPanel { border: 1px solid #397589; border-radius: 10px; background: rgba(255, 255, 255, 160); }"
                  "QLabel#serverStatusLabel, QLabel#dashboardStatusLabel { color: #3d3d3d; font-size: 12px; }"
                  "QPushButton#profileFriendButton { border-color: #2b9bb2; color: #287e91; }"
                  "QLineEdit { min-height: 36px; border: 1px solid #cfcfc9; border-radius: 7px; padding: 0 10px; background: rgba(255, 255, 255, 160); }"
                  "QTextEdit { border: 1px solid #d8d8d2; border-radius: 8px; background: rgba(255, 255, 255, 160); }");

    connect(ui->continueButton, &QPushButton::clicked, this, &MainWindow::onContinue);
    connect(ui->accessLoginButton, &QPushButton::clicked, this, &MainWindow::onOpenLogin);
    connect(ui->accessRegisterButton, &QPushButton::clicked, this, &MainWindow::onOpenRegister);
    connect(ui->accessDeleteButton, &QPushButton::clicked, this, &MainWindow::onOpenDelete);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::onExit);
    connect(ui->loginBackButton, &QPushButton::clicked, this, &MainWindow::onBackToAccess);
    connect(ui->registerBackButton, &QPushButton::clicked, this, &MainWindow::onBackToAccess);
    connect(ui->deleteBackButton, &QPushButton::clicked, this, &MainWindow::onBackToAccess);
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLogin);
    connect(ui->registerButton, &QPushButton::clicked, this, &MainWindow::onRegister);
    connect(ui->deleteAccountButton, &QPushButton::clicked, this, &MainWindow::onDeleteAccount);
    connect(profileFriendButton, &QPushButton::clicked, this, &MainWindow::onProfileFriendAction);
    connect(ui->showFriendsButton, &QPushButton::clicked, this, &MainWindow::onShowFriends);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchUser);
    connect(ui->friendTargetEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearchUser);
    connect(ui->friendTargetEdit, &QLineEdit::textChanged, this, [this](const QString&) { refreshUserSuggestions(); });
    connect(ui->bfsButton, &QPushButton::clicked, this, &MainWindow::onRunBFS);
    connect(ui->dfsButton, &QPushButton::clicked, this, &MainWindow::onRunDFS);
    connect(ui->mutualButton, &QPushButton::clicked, this, &MainWindow::onShowMutualFriends);
    connect(ui->suggestButton, &QPushButton::clicked, this, &MainWindow::onSuggestFriends);
    connect(ui->logoutButton, &QPushButton::clicked, this, &MainWindow::onLogout);
}

void MainWindow::onContinue() { ui->pages->setCurrentWidget(ui->accessPage); }
void MainWindow::onOpenLogin() { ui->pages->setCurrentWidget(ui->loginPage); }
void MainWindow::onOpenRegister() { ui->pages->setCurrentWidget(ui->registerPage); }
void MainWindow::onOpenDelete() { ui->pages->setCurrentWidget(ui->deletePage); }
void MainWindow::onBackToAccess() { ui->pages->setCurrentWidget(ui->accessPage); }
void MainWindow::onExit() { close(); }

MainWindow::~MainWindow() {
    dataStore.save(socialGraph, accounts);
    delete ui;
}

const Account* MainWindow::findAccount(const QString& username) const {
    for (const Account& account : accounts) {
        if (QString::fromStdString(account.username) == username) return &account;
    }
    return nullptr;
}

const Account* MainWindow::findAccountByUserId(int userId) const {
    for (const Account& account : accounts) {
        if (account.userId == userId) return &account;
    }
    return nullptr;
}

int MainWindow::findUserId(const QString& searchTerm) const {
    List<User> users = socialGraph.getAllUsers();
    for (int index = 0; index < users.size(); index++) {
        if (QString::fromStdString(users[index].getName()) == searchTerm) return users[index].getId();
    }
    for (const Account& account : accounts) {
        if (QString::fromStdString(account.uid) == searchTerm) return account.userId;
    }
    return -1;
}

void MainWindow::showStatus(const QString& message) {
    ui->authStatusLabel->setText(message);
    ui->dashboardStatusLabel->setText(message);
    ui->outputEdit->append(message);
}

void MainWindow::showIds(const QString& label, const List<int>& ids) {
    QString result = label;
    if (ids.isEmpty()) {
        ui->outputEdit->append(result + "None");
        return;
    }
    for (int index = 0; index < ids.size(); index++) {
        const User* user = socialGraph.findUser(ids[index]);
        if (user != nullptr) result += QString::fromStdString(user->getName());
        if (index + 1 < ids.size()) result += ", ";
    }
    ui->outputEdit->append(result);
}

void MainWindow::openSearchMode(SearchMode mode) {
    searchMode = mode;
    ui->friendTargetEdit->clear();
    ui->friendTargetEdit->show();
    ui->friendTargetEdit->setFocus();

    ui->friendTargetEdit->setPlaceholderText("Search users by name or UID...");
    refreshUserSuggestions();
}

void MainWindow::closeSearchMode() {
    searchMode = SearchMode::None;
    ui->friendTargetEdit->clear();
    ui->friendTargetEdit->hide();
}

void MainWindow::refreshUserSuggestions() {
    QStringList suggestions;
    const QString query = ui->friendTargetEdit->text().trimmed();
    List<User> users = socialGraph.getAllUsers();
    for (int index = 0; index < users.size(); index++) {
        const QString name = QString::fromStdString(users[index].getName());
        const Account* account = findAccountByUserId(users[index].getId());
        const QString uid = account == nullptr ? QString() : QString::fromStdString(account->uid);
        if (query.isEmpty() || name.contains(query, Qt::CaseInsensitive) || uid.contains(query, Qt::CaseInsensitive)) suggestions.append(name);
    }
    userSuggestionModel->setStringList(suggestions);
}

void MainWindow::showUserProfile(int userId) {
    const User* user = socialGraph.findUser(userId);
    if (user == nullptr) return;
    viewedUserId = userId;
    const Account* account = findAccountByUserId(userId);
    const QString uid = account == nullptr ? "unknown" : QString::fromStdString(account->uid);
    profileViewLabel->setText(QString("%1\nUID: %2").arg(QString::fromStdString(user->getName())).arg(uid));
    profileFriendButton->setText(socialGraph.areFriends(currentUserId, userId) ? "Remove Friend" : "Add Friend");
    profileFriendButton->setEnabled(currentUserId != userId);
    profileViewLabel->show();
    profileFriendButton->show();
    ui->outputEdit->append(QString("Viewing %1's profile.").arg(QString::fromStdString(user->getName())));
}

void MainWindow::onLogin() {
    const QString username = ui->loginUsernameEdit->text().trimmed();
    const QString password = ui->loginPasswordEdit->text();
    const Account* account = findAccount(username);
    if (account == nullptr || QString::fromStdString(account->password) != password) {
        showStatus("Invalid username or password.");
        return;
    }
    currentUserId = account->userId;
    const User* user = socialGraph.findUser(currentUserId);
    ui->logoLabel->hide();
    headerWidget->show();
    const QString displayName = QString::fromStdString(user->getName());
    headerAccountLabel->setText(displayName.toUpper());
    headerAvatarLabel->setText(displayName.left(1).toUpper());
    ui->profileNameLabel->setText(QString::fromStdString(user->getName()).toUpper());
    ui->loggedInLabel->setText(QString("Welcome, %1. Choose an action from the menu to get started.").arg(QString::fromStdString(user->getName())));
    ui->pages->setCurrentWidget(ui->socialPage);
    ui->outputEdit->clear();
    ui->outputEdit->append("Login successful.");
}

void MainWindow::onRegister() {
    const QString username = ui->registerUsernameEdit->text().trimmed();
    const QString password = ui->registerPasswordEdit->text();
    const QString displayName = ui->displayNameEdit->text().trimmed();
    const QString uid = ui->uidEdit->text().trimmed();
    if (username.isEmpty() || password.isEmpty() || displayName.isEmpty() || uid.isEmpty()) {
        showStatus("All registration fields are required.");
        return;
    }
    if (findAccount(username) != nullptr) {
        showStatus("That username is already taken.");
        return;
    }
    for (const Account& account : accounts) {
        if (QString::fromStdString(account.uid) == uid) {
            showStatus("That UID is already taken.");
            return;
        }
    }
    const int userId = nextUserId++;
    accounts.push_back({username.toStdString(), password.toStdString(), uid.toStdString(), userId});
    std::string name = displayName.toStdString();
    socialGraph.addUser(userId, name.c_str());
    dataStore.save(socialGraph, accounts);
    showStatus("Registration successful. You can now log in.");
    ui->pages->setCurrentWidget(ui->loginPage);
}

void MainWindow::onDeleteAccount() {
    const QString username = ui->deleteUsernameEdit->text().trimmed();
    const QString password = ui->deletePasswordEdit->text();
    int accountIndex = -1;
    for (int index = 0; index < static_cast<int>(accounts.size()); index++) {
        if (QString::fromStdString(accounts[index].username) == username && QString::fromStdString(accounts[index].password) == password) {
            accountIndex = index;
            break;
        }
    }
    if (accountIndex == -1) {
        showStatus("Invalid username or password. Account was not deleted.");
        return;
    }
    socialGraph.removeUser(accounts[accountIndex].userId);
    accounts.erase(accounts.begin() + accountIndex);
    dataStore.save(socialGraph, accounts);
    showStatus("Account and friendships deleted.");
}

void MainWindow::onProfileFriendAction() {
    if (viewedUserId == -1 || viewedUserId == currentUserId) return;
    const bool removing = socialGraph.areFriends(currentUserId, viewedUserId);
    const bool changed = removing ? socialGraph.removeFriendship(currentUserId, viewedUserId) : socialGraph.addFriendship(currentUserId, viewedUserId);
    if (changed) {
        dataStore.save(socialGraph, accounts);
        profileFriendButton->setText(removing ? "Add Friend" : "Remove Friend");
        showStatus(removing ? "Friend removed." : "Friend added.");
    }
}

void MainWindow::onShowFriends() { showIds("Your friends: ", socialGraph.getFriends(currentUserId)); }

void MainWindow::onSearchUser() {
    if (searchMode != SearchMode::SearchUser) {
        openSearchMode(SearchMode::SearchUser);
        return;
    }

    const int userId = findUserId(ui->friendTargetEdit->text().trimmed());
    if (userId == -1) { showStatus("No user found with that display name or UID."); return; }
    showUserProfile(userId);
    closeSearchMode();
}

void MainWindow::onRunBFS() { showIds("BFS order: ", socialGraph.bfsTraversal(currentUserId)); }
void MainWindow::onRunDFS() { showIds("DFS order: ", socialGraph.dfsTraversal(currentUserId)); }

void MainWindow::onShowMutualFriends() {
    const int otherUserId = findUserId(ui->friendTargetEdit->text().trimmed());
    if (otherUserId == -1) { showStatus("No user found with that display name or UID."); return; }
    showIds("Mutual friends: ", socialGraph.getMutualFriends(currentUserId, otherUserId));
}

void MainWindow::onSuggestFriends() {
    showIds("Friend suggestions: ", socialGraph.suggestFriends(currentUserId));
}

void MainWindow::onLogout() {
    currentUserId = -1;
    closeSearchMode();
    ui->logoLabel->show();
    headerWidget->hide();
    profileViewLabel->hide();
    profileFriendButton->hide();
    ui->pages->setCurrentWidget(ui->accessPage);
    ui->loginUsernameEdit->clear();
    ui->loginPasswordEdit->clear();
    showStatus("You have been logged out.");
}
