#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSortFilterProxyModel>

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
    void loadHistory();
    void onReLoginButtonClicked();
    void loadDatabaseTables();
    void onTableSelected(const QModelIndex &index);
    void addRow();
    void onSearchText();

private:
    Ui::UserWindow *ui;
    DatabaseQueries dbQueries;
    MainWindow *mainWindow;

    QSqlTableModel *tableModel = nullptr;  // Додано змінну tableModel
    QSortFilterProxyModel *proxyModel;
};

#endif // USERWINDOW_H
