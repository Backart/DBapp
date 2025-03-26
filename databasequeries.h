#ifndef DATABASEQUERIES_H
#define DATABASEQUERIES_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QJsonObject>
#include <QJsonDocument>

#include "errormessages.h"

#include "securityutils.h"

class DatabaseQueries
{
public:
    explicit DatabaseQueries(QSqlDatabase& db);

    bool authenticateUser(const QString& username, const QString& password, int& role_id, QString& errorMessage);
    bool registerUser(const QString& username, const QString& password, const QString& confirmPassword, int& role_id, QString& errorMessage);

    bool windowAdmin(QSqlQueryModel* model, QString& errorMessage);
    bool windowUser(QSqlTableModel* model, QString& errorMessage);
    bool tableHistory(QSqlQueryModel* model, QString& errorMessage);

    QSqlDatabase& getDatabase() { return database; }

    bool updateUsername(const QString& oldUsername, const QString& newUsername);
    bool updatePassword(const QString& username, const QString& newPassword);
    bool isUsernameExist(const QString& username);

private:
    QSqlDatabase& database;
};

#endif // DATABASEQUERIES_H
