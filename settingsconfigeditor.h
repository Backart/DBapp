#ifndef SETTINGSCONFIGEDITOR_H
#define SETTINGSCONFIGEDITOR_H

#include <QWidget>

namespace Ui {
class SettingsConfigEditor;
}

class SettingsConfigEditor : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsConfigEditor(QWidget *parent = nullptr);
    ~SettingsConfigEditor();

private:
    Ui::SettingsConfigEditor *ui;
};

#endif // SETTINGSCONFIGEDITOR_H
