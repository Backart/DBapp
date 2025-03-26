#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include <QFile>
#include <QApplication>

#include "dbconnectionmanager.h"
#include "settingsconfigeditor.h"
#include "databasequeries.h"
#include "errormessages.h"

namespace Ui {
class settingwindow;
}

class settingwindow : public QDialog
{
    Q_OBJECT

public:
    explicit settingwindow(QSqlDatabase& db, QWidget *parent = nullptr);
    ~settingwindow();


private slots:
    void onSaveClicked();
    void onCloseClicked();
    void onDeleteAccountClicked();

    void onDarkChecked();
    void onLightChecked();

private:
    Ui::settingwindow *ui;

    DatabaseQueries dbQueries;

    void applyTheme(const QString &theme);
    void loadTheme();
};

#endif // SETTINGWINDOW_H
