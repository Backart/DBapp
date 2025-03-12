#include "settingsconfigeditor.h"
#include "ui_settingsconfigeditor.h"

SettingsConfigEditor::SettingsConfigEditor(QSqlDatabase& db, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsConfigEditor)
    ,dbQueries(db)
{
    ui->setupUi(this);

    // 1. Отримуємо шлях до файлу
    configPath = getConfigFilePath();
    // 2. Якщо файл не існує, створюємо його з дефолтними значеннями
    ensureConfigFileExists();
    // 3. Завантажуємо поточні налаштування в UI
    loadCurrentConfig();

    connect(ui->pushButton_Save, &QPushButton::clicked, this, &SettingsConfigEditor::onSaveButtonClicked);
    connect(ui->pushButton_Cancel, &QPushButton::clicked, this, &SettingsConfigEditor::onCancelButtonClicked);
}

SettingsConfigEditor::~SettingsConfigEditor()
{
    delete ui;
}

// Функція отримання правильного шляху до файлу конфігурації
QString SettingsConfigEditor::getConfigFilePath()
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(configDir);
    return QDir::cleanPath(configDir + "/dataConnectConfig.ini");
}

// Функція створення файлу, якщо його немає
void SettingsConfigEditor::ensureConfigFileExists()
{
    QFile configFile(configPath);

    if (!configFile.exists()) {
        QSettings settings(configPath, QSettings::IniFormat);
        settings.setValue("Database/host", "127.0.0.1");
        settings.setValue("Database/port", 5432);
        settings.setValue("Database/name", "");
        settings.setValue("Database/user", "postgres");
        settings.setValue("Database/password", "");
    }
}

// Завантажуємо поточні налаштування в UI
void SettingsConfigEditor::loadCurrentConfig()
{
    QSettings settings(configPath, QSettings::IniFormat);

    ui->lineEdit_Host->setText(settings.value("Database/host", "127.0.0.1").toString());
    ui->lineEdit_Port->setText(settings.value("Database/port", 5432).toString());
    ui->lineEdit_DBname->setText(settings.value("Database/name", "").toString());
    ui->lineEdit_DBuser->setText(settings.value("Database/user", "postgres").toString());
    ui->lineEdit_Password->setText(settings.value("Database/password", "").toString());
}

// Збереження змін
void SettingsConfigEditor::onSaveButtonClicked()
{

    QString host = ui->lineEdit_Host->text();
    QString portStr = ui->lineEdit_Port->text();
    QString dbName = ui->lineEdit_DBname->text();
    QString user = ui->lineEdit_DBuser->text();
    QString password = ui->lineEdit_Password->text();

    if (host.isEmpty() || portStr.isEmpty() || dbName.isEmpty() || user.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please fill in all fields!");
        return;
    }

    // Конвертація порту в int
    bool ok = false;
    int port = portStr.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Warning", "Port must be a valid number!");
        return;
    }

    QSettings settings(configPath, QSettings::IniFormat);

    settings.setValue("Database/host", host);
    settings.setValue("Database/port", port);
    settings.setValue("Database/name", dbName);
    settings.setValue("Database/user", user);
    settings.setValue("Database/password", password);

    QMessageBox::information(this, "Saved", "Database configuration saved successfully!");

    // Перевідкрити базу, щоб зміни застосувались одразу:
    DBConnectionManager::instance().closeDatabase();
    DBConnectionManager::instance().openDatabase();

    accept();
}

void SettingsConfigEditor::onCancelButtonClicked()
{
    // Закриваємо без збереження
    reject();
}
