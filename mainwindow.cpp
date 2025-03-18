#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Opening and connecting to the databases
    if (!DBConnectionManager::instance().openDatabase()) {
        qDebug() << "Databased not connect: " << DBConnection.lastError().text();
        qDebug() << DBConnection.lastError().driverText();
        qDebug() << DBConnection.lastError().databaseText();
        qDebug() << QSqlDatabase::drivers();
    }

    // Loading and displaying the login icon image
    QPixmap pix ("/home/monk/Documents/Projects/QT_Work/Education_QT/DBapp/img/login_icon.png");
    ui->label_picLogin->setPixmap(pix.scaled(250,250, Qt::KeepAspectRatio));
    // Connecting buttons to the corresponding slots
    connect(ui->pushButton_Login, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(ui->pushButton_Cancel, &QPushButton::clicked, this, &MainWindow::onCancelClicked);
    connect(ui->pushButton_SignUp, &QPushButton::clicked, this, &MainWindow::onSignUp);
    connect(ui->pushButton_DBconnect, &QPushButton::clicked, this, &MainWindow::onDBConfiguration);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoginClicked()
{
    QString username = ui->lineEdit_UserName->text();
    QString password = ui->lineEdit_Password->text();

    int role_id;
    QString errorMessage;

    UserAuthorization auth(this);
    if (auth.authenticateUser(username, password, role_id, errorMessage))
        this->close();  // close головне вікно після успішного входу
}

QString MainWindow::getUsername() const {
    return ui->lineEdit_UserName->text();
}

// Slot for the "Cancel" button
void MainWindow::onCancelClicked()
{
    this->close();
}

void MainWindow::onSignUp()
{
    // Створюємо об'єкт вікна реєстрації
    DialogRegisterWindow *registerWindow = new DialogRegisterWindow(DBConnection, this);
    // Відкриваємо вікно реєстрації
    registerWindow->exec();  // Викликає модальне вікно (не дає взаємодіяти з іншими вікнами, поки воно відкрите)
}

void MainWindow::onDBConfiguration()
{
    // Створюємо об'єкт вікна configWindow
    SettingsConfigEditor *configWindow = new SettingsConfigEditor(DBConnection, this);
    // Відкриваємо вікно configWindow
    configWindow->exec();  // Викликає модальне вікно (не дає взаємодіяти з іншими вікнами, поки воно відкрите)
}
