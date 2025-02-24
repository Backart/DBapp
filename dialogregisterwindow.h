#ifndef DIALOGREGISTERWINDOW_H
#define DIALOGREGISTERWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql/QtSql>
#include <QSqlError>
#include <QSqlQuery>

#include "databasequeries.h"
#include "errormessages.h"

namespace Ui {
class DialogRegisterWindow;
}

class DialogRegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRegisterWindow(QSqlDatabase& db, QWidget *parent = nullptr);
    ~DialogRegisterWindow();

private slots:
    void onRegister();
    void onCancelClicked();

private:
    Ui::DialogRegisterWindow *ui;
    DatabaseQueries dbQueries; // Додаємо об'єкт для запитів до БД
};

#endif // DIALOGREGISTERWINDOW_H
