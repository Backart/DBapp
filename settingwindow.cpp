#include "settingwindow.h"
#include "ui_settingwindow.h"

settingwindow::settingwindow(QSqlDatabase& db,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::settingwindow)
    , dbQueries(db)
{
    ui->setupUi(this);

    loadTheme();

    // Підключаємо сигнали кнопок до слотів
    connect(ui->pushButton_Save, &QPushButton::clicked, this, &settingwindow::onSaveClicked);
    connect(ui->pushButton_Close, &QPushButton::clicked, this, &settingwindow::onCloseClicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &settingwindow::onDeleteAccountClicked);

    // Підключаємо чекбокси до слотів
    connect(ui->checkBox_Dark, &QCheckBox::clicked, this, &settingwindow::onDarkChecked);
    connect(ui->checkBox_Light, &QCheckBox::clicked, this, &settingwindow::onLightChecked);
}

settingwindow::~settingwindow()
{
    delete ui;
}



void settingwindow::onDeleteAccountClicked()
{
    // Логіка видалення користувача
    /*
    if (!dbQueries.deleteUser(...)) {
        QMessageBox::critical(this, "Error", "Failed to delete user!");
        return;
    }
    */

    QMessageBox::information(this, "Deleted", "User account has been deleted!");
    this->close();
}

// ---

void settingwindow::onSaveClicked() {

    QString currentUsername = dbQueries.getCurrentUsername(); // Отримання поточного імені користувача
    qDebug() << "currentUsername in onSaveClicked: " << currentUsername;
    ui->lineEdit_UserName->setText(dbQueries.getCurrentUsername());

    QString newUsername = ui->lineEdit_UserName->text().trimmed();
    QString newPassword = ui->lineEdit_ChangePassword->text().trimmed();
    QString errorMessage;

    if (newUsername.isEmpty() && newPassword.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No changes detected.");
        return;
    }

    QSqlQueryModel model;

    // Оновлення імені користувача
    if (!newUsername.isEmpty() && newUsername != currentUsername) {
        if (dbQueries.isUsernameExist(&model, errorMessage, newUsername)) {
            QMessageBox::warning(this, "Warning", "Username already exists!");
            return;
        }
        if (!dbQueries.updateUsername(&model, errorMessage, currentUsername, newUsername)) {
            QMessageBox::critical(this, "Error", "Failed to update username: " + errorMessage);
            return;
        }
        currentUsername = newUsername; // Оновлення імені користувача в поточній сесії
    }

    // Оновлення пароля
    if (!newPassword.isEmpty()) {
        if (!dbQueries.updatePassword(&model, errorMessage, currentUsername, newPassword)) {
            QMessageBox::critical(this, "Error", "Failed to update password: " + errorMessage);
            return;
        }
    }

    QMessageBox::information(this, "Success", "Settings saved successfully!");
}

void settingwindow::onCloseClicked()
{
    this->close();
}

// ---

void settingwindow::loadTheme()
{
    QSettings settings("MyCompany", "MyApp");
    QString theme = settings.value("theme", "light").toString();

    if (theme == "dark") {
        ui->checkBox_Dark->setChecked(true);
        ui->checkBox_Light->setChecked(false);
    } else {
        ui->checkBox_Light->setChecked(true);
        ui->checkBox_Dark->setChecked(false);
    }

    applyTheme(theme);
}

void settingwindow::applyTheme(const QString &theme)
{
    if (theme == "dark") {
        qApp->setStyle("Fusion");  // Використовує стандартний стиль Fusion, який підтримує темну тему
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::white);

        qApp->setPalette(darkPalette);
    } else {
        qApp->setStyle("Fusion");  // Теж використовує стиль Fusion для світлої теми
        QPalette lightPalette;
        lightPalette.setColor(QPalette::Window, Qt::white);
        lightPalette.setColor(QPalette::WindowText, Qt::black);
        lightPalette.setColor(QPalette::Base, Qt::white);
        lightPalette.setColor(QPalette::AlternateBase, Qt::lightGray);
        lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
        lightPalette.setColor(QPalette::ToolTipText, Qt::black);
        lightPalette.setColor(QPalette::Text, Qt::black);
        lightPalette.setColor(QPalette::Button, Qt::white);
        lightPalette.setColor(QPalette::ButtonText, Qt::black);
        lightPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        lightPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        lightPalette.setColor(QPalette::HighlightedText, Qt::black);

        qApp->setPalette(lightPalette);
    }
}

void settingwindow::onDarkChecked()
{
    if (ui->checkBox_Dark->isChecked()) {
        ui->checkBox_Light->setChecked(false);
        QSettings settings("MyCompany", "MyApp");
        settings.setValue("theme", "dark");
        applyTheme("dark");
    }
}


void settingwindow::onLightChecked()
{
    if (ui->checkBox_Light->isChecked()) {
        ui->checkBox_Dark->setChecked(false);
        QSettings settings("MyCompany", "MyApp");
        settings.setValue("theme", "light");
        applyTheme("light");
    }
}
