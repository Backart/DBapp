#include "userwindow.h"
#include "mainwindow.h"
#include "databasequeries.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(QSqlDatabase& db, MainWindow *mainWindow, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
    , dbQueries(db)
    , mainWindow(mainWindow)
{
    ui->setupUi(this);

    loadUserOrders();
    loadHistory();
    onReLoginButtonClicked();

    connect(ui->pushButton_UpdateDB, &QPushButton::clicked, this, &UserWindow::loadUserOrders);
    connect(ui->pushButton_re_login_singup, &QPushButton::clicked, this, &UserWindow::onReLoginButtonClicked);

}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::loadUserOrders() {
    qDebug() << "Username in UserWindow: user";

    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("orders");  // Встановлюємо таблицю
    model->select();  // Завантажуємо дані

    ui->tableView_DB->setModel(model);
    ui->tableView_DB->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    // Зберігаємо зміни
    if (!model->submitAll()) {
        qDebug() << "Failed to save changes:" << model->lastError().text();
    } else {
        qDebug() << "Changes saved successfully!";
    }
}

void UserWindow::onReLoginButtonClicked() {
    if (mainWindow) {
        mainWindow->show();
    }
    this->close();
}

void UserWindow::loadHistory()
{
    qDebug() << "Username in UserWindow: history";

    QString errorMessage;
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("history");
    model->select();  // Load data from the history table

    ui->tableView_history->setModel(model);
    ui->tableView_history->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    if (model->lastError().isValid()) {
        errorMessage = model->lastError().text();
        qDebug() << "Error loading history:" << errorMessage;
    } else {
        qDebug() << "History loaded successfully!";
    }

    // Отримання поточного користувача
    QString currentUser = mainWindow->getUsername();
    qDebug() << "User:" << currentUser;

    // Перевірка, чи база даних відкрита
    if (!dbQueries.getDatabase().isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    // Використовуємо dbQueries для створення запиту
    QSqlQuery query(dbQueries.getDatabase());

    // Start a transaction
    if (!dbQueries.getDatabase().transaction()) {
        qDebug() << "Error starting transaction:" << dbQueries.getDatabase().lastError().text();
        return;
    }

    query.prepare(R"(
        INSERT INTO history (user_name)
        VALUES (?)
    )");

    query.addBindValue(currentUser);  // Використовуємо currentUser

    if (!query.exec()) {
        qDebug() << "Error inserting history log:" << query.lastError().text();
        qDebug() << "Query:" << query.lastQuery();
        dbQueries.getDatabase().rollback(); // Відкат транзакції у разі помилки
    } else {
        dbQueries.getDatabase().commit(); // Підтвердження транзакції
        qDebug() << "History log inserted successfully!";
    }
}
