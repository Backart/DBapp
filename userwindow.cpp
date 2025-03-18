#include "userwindow.h"
#include "mainwindow.h"
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

    // Підписуємося на сигнал dataChanged
    connect(model, &QSqlTableModel::dataChanged, this, &UserWindow::onDataChanged);
    qDebug() << "Connected dataChanged signal to onDataChanged slot.";

    qDebug() << "BEFORE loadUserOrders() call:";
    qDebug() << " - dbQueries pointer:" << &dbQueries;
    qDebug() << " - Database open state:" << dbQueries.getDatabase().isOpen();

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

    qDebug() << "BEFORE loadHistory() call:";
    qDebug() << " - dbQueries pointer:" << &dbQueries;
    qDebug() << " - Database open state:" << dbQueries.getDatabase().isOpen();

    if (model->lastError().isValid()) {
        errorMessage = model->lastError().text();
        qDebug() << "Error loading history:" << errorMessage;
    } else {
        qDebug() << "History loaded successfully!";
    }
}

void UserWindow::onDataChanged() {

    qDebug() << "Data changed detected!";

    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(sender());
    if (!model) {
        qDebug() << "Failed to cast sender to QSqlTableModel. Sender type:" << sender()->metaObject()->className();
        return;
    }

    QString tableName = model->tableName();
    qDebug() << "Table changed:" << tableName;

    // Отримання поточного користувача
    QString currentUser = mainWindow->getUsername();
    qDebug() << "User:" << currentUser;

    qDebug() << "BEFORE logHistory() call:";
    qDebug() << " - dbQueries pointer:" << &dbQueries;
    qDebug() << " - Database open state:" << dbQueries.getDatabase().isOpen();
    qDebug() << " - Current user:" << currentUser;
    qDebug() << " - Table name:" << tableName;

    // Перевірка, чи база даних відкрита
    if (!dbQueries.getDatabase().isOpen()) {
        qDebug() << "Database is not open! Trying to open it...";
        if (!dbQueries.getDatabase().open()) {
            qDebug() << "Failed to open database:" << dbQueries.getDatabase().lastError().text();
            return;
        }
    }

    // Запис у лог
    dbQueries.logHistory(currentUser, tableName);
}
