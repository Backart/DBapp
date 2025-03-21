#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPixmap>
#include <QtSql/QtSql>

#include "dialogregisterwindow.h"
#include "adminwindow.h"
#include "userwindow.h"

#include "dbconnectionmanager.h"
#include "userauthorization.h"

#include "settingsconfigeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getUsername() const;

private slots:
    void onLoginClicked();    // Слот для кнопки "Login"
    void onCancelClicked();   // Слот для кнопки "Cancel"
    void onSignUp();
    void onDBConfiguration();

    //void openUserInterface(int role_id, QString username);

private:
    Ui::MainWindow *ui;
    QSqlDatabase DBConnection;

};
#endif // MAINWINDOW_H
