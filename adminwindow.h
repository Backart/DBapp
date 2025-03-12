#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include <QtSql/QtSql>
#include <QSqlError>
#include <QSqlQuery>

#include "databasequeries.h"

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QSqlDatabase& db, QWidget *parent = nullptr);
    ~AdminWindow();



private:
    Ui::AdminWindow *ui;

    void loadAdminTables();

    DatabaseQueries dbQueries; // Додаємо об'єкт для запитів до БД
};

#endif // ADMINWINDOW_H
