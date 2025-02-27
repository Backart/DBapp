#ifndef SETTINGSCONFIGEDITOR_H
#define SETTINGSCONFIGEDITOR_H

#include <QDialog>
#include <QSettings>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>

#include "dbconnectionmanager.h"
#include "databasequeries.h"
#include "errormessages.h"

namespace Ui {
class SettingsConfigEditor;
}

class SettingsConfigEditor : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsConfigEditor(QSqlDatabase& db, QWidget *parent = nullptr);
    ~SettingsConfigEditor();

private slots:

    void onSaveButtonClicked();
    void onCancelButtonClicked();

    void ensureConfigFileExists();

    QString getConfigFilePath();


private:
    Ui::SettingsConfigEditor *ui;
    DatabaseQueries dbQueries;

    QString configPath;

    void loadCurrentConfig();
};

#endif // SETTINGSCONFIGEDITOR_H
