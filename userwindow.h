#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include <QtSql/QtSql>
#include <QSqlError>
#include <QSqlQuery>

#include "databasequeries.h"

namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QSqlDatabase& db, QWidget *parent = nullptr);
    ~UserWindow();

private:
    Ui::UserWindow *ui;

    void loadUserOrders();

    void loadHistory();

    void onReLoginButtonClicked();

    DatabaseQueries dbQueries; // Додаємо об'єкт для запитів до БД
};

#endif // USERWINDOW_H
