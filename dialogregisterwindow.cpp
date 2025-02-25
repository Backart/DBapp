#include "dialogregisterwindow.h"
#include "ui_dialogregisterwindow.h"

DialogRegisterWindow::DialogRegisterWindow(QSqlDatabase& db, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::DialogRegisterWindow)
    ,dbQueries(db)
{
    ui->setupUi(this);

    connect(ui->pushButton_Register, &QPushButton::clicked, this, &DialogRegisterWindow::onRegister);
    connect(ui->pushButton_Cancel, &QPushButton::clicked, this, &DialogRegisterWindow::onCancelClicked);

}

DialogRegisterWindow::~DialogRegisterWindow()
{
    delete ui;
}

void DialogRegisterWindow::onRegister() {
    // Отримуємо введені дані
    QString username = ui->lineEdit_UserName->text();
    QString password = ui->lineEdit_Password->text();
    QString confirmPassword = ui->lineEdit_ConfirmPassword->text(); // Поле для підтвердження пароля

    // role_id за замовчуванням 2 (звичайний користувач). Адмін може вручну встановити 1.

    // Якщо поточний адміністратор хоче створити іншого адміністратора
    /*QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Assign Admin Role",
                                  "Do you want to assign this user as an admin?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        role_id = 1; // Роль адміністратора
    }*/

    int role_id = 2;  // Роль за замовчуванням - звичайний користувач
    QString errorMessage;

    if (dbQueries.registerUser(username, password, confirmPassword, role_id, errorMessage))
        this->accept();
}

void DialogRegisterWindow::onCancelClicked()
{
    // Clear input fields for username and password
    ui->lineEdit_UserName->clear();
    ui->lineEdit_Password->clear();
    ui->lineEdit_ConfirmPassword->clear();
}
