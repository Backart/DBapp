#ifndef DATABASEQUERIES_H
#define DATABASEQUERIES_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlTableModel>

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

private:
    QSqlDatabase& database;
};

#endif // DATABASEQUERIES_H
