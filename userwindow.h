#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include <QtSql/QtSql>
#include <QSqlError>
#include <QSqlQuery>

#include "databasequeries.h"

class MainWindow;  // Оголошення класу (форвард-декларація)

namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QSqlDatabase& db, MainWindow *mainWindow, QWidget *parent = nullptr);
    ~UserWindow();

private slots:
    void loadUserOrders();
    void loadHistory();
    void onReLoginButtonClicked();

private:
    Ui::UserWindow *ui;
    DatabaseQueries dbQueries;
    MainWindow *mainWindow;
};

#endif // USERWINDOW_H
