#ifndef DBCONNECTIONMANAGER_H
#define DBCONNECTIONMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>

class DBConnectionManager
{
public:
    static DBConnectionManager& instance();
    QSqlDatabase getDatabase();
    bool openDatabase();
    void closeDatabase();

private:
    DBConnectionManager();
    ~DBConnectionManager();
    QSqlDatabase db;
};

#endif // DBCONNECTIONMANAGER_H
