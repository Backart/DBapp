#ifndef USERAUTHORIZATION_H
#define USERAUTHORIZATION_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

#include "dbconnectionmanager.h"
#include "adminwindow.h"
#include "userwindow.h"
#include "errormessages.h"
#include "databasequeries.h"

#include "bcryptcpp.h"
#include "databasequeries.h"

class UserAuthorization
{
public:
    explicit UserAuthorization(MainWindow *mainWindow);
    bool authenticateUser(const QString& username, const QString& password, int& role_id, QString& errorMessage);

private:
    bool validateInput(const QString& username, const QString& password, QString& errorMessage);
    void openUserInterface(int role_id, const QString& username);

    QSqlDatabase database;
    DatabaseQueries* dbQueries;

    MainWindow *mainWindow;

    QString hashPassword(const QString& password);
};

#endif // USERAUTHORIZATION_H
