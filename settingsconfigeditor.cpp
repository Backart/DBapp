#include "settingsconfigeditor.h"
#include "ui_settingsconfigeditor.h"

SettingsConfigEditor::SettingsConfigEditor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsConfigEditor)
{
    ui->setupUi(this);
}

SettingsConfigEditor::~SettingsConfigEditor()
{
    delete ui;
}
