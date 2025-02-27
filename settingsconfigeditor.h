#ifndef SETTINGSCONFIGEDITOR_H
#define SETTINGSCONFIGEDITOR_H

#include <QDialog>

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
    void onDBConfiguration();

private:
    Ui::SettingsConfigEditor *ui;
    DatabaseQueries dbQueries;
};

#endif // SETTINGSCONFIGEDITOR_H
