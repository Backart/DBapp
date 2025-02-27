#include "settingsconfigeditor.h"
#include "ui_settingsconfigeditor.h"

SettingsConfigEditor::SettingsConfigEditor(QSqlDatabase& db, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsConfigEditor)
    ,dbQueries(db)
{
    ui->setupUi(this);

}

SettingsConfigEditor::~SettingsConfigEditor()
{
    delete ui;
}

void SettingsConfigEditor::onDBConfiguration()
{
    // Додайте код для обробки налаштувань БД
    qDebug() << "DB Configuration settings opened.";
}
