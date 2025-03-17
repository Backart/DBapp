#include "userauthorization.h"

UserAuthorization::UserAuthorization(MainWindow *mainWindow) {
    this->mainWindow = mainWindow;
    database = DBConnectionManager::instance().getDatabase();
    dbQueries = new DatabaseQueries(database);
}

bool UserAuthorization::authenticateUser(const QString& username, const QString& password, int& role_id, QString& errorMessage) {
    if (!validateInput(username, password, errorMessage))
        return false;

    if (!dbQueries->authenticateUser(username, password, role_id, errorMessage))
        return false;

    errorMessage = "Login successful!\nWelcome , ";
    ErrorMessages::showMessage(ErrorMessages::ERROR_200, errorMessage + username);
    openUserInterface(role_id, username);
    return true;
}

bool UserAuthorization::validateInput(const QString& username, const QString& password, QString& errorMessage) {
    if (username.isEmpty() || password.isEmpty()) {
        errorMessage = "Please enter both username and password.";
        ErrorMessages::showMessage(ErrorMessages::ERROR_400, errorMessage);
        return false;
    }

    return true;
}

void UserAuthorization::openUserInterface(int role_id, const QString& username) {
    if (role_id == 1) {
        AdminWindow *adminWindow = new AdminWindow(database);
        adminWindow->show();
    } else if (role_id == 2) {
        UserWindow *userWindow = new UserWindow(database, mainWindow);
        userWindow->show();
    }
}
