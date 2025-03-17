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
    QSqlDatabase& getDatabase();

    bool authenticateUser(const QString& username, const QString& password, int& role_id, QString& errorMessage);
    bool registerUser(const QString& username, const QString& password, const QString& confirmPassword, int& role_id, QString& errorMessage);

    bool windowAdmin(QSqlQueryModel* model, QString& errorMessage);
    bool windowUser(QSqlTableModel* model, QString& errorMessage);
    bool tableHistory(QSqlQueryModel* model, QString& errorMessage);

    void logHistory(const QString &action, const QString &table, int recordId,
                    const QMap<QString, QVariant> &oldData, const QMap<QString, QVariant> &newData);

private:
    QSqlDatabase& database;
};

#endif // DATABASEQUERIES_H
