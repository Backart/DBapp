#include "userwindow.h"
#include "mainwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(QSqlDatabase& db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
    ,dbQueries(db)
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

void UserWindow::loadUserOrders()
{
    qDebug() << "Username in UserWindow: user";

    QString errorMessage;
    QSqlTableModel *model = new QSqlTableModel();

    if (dbQueries.windowUser(model, errorMessage)) {
        ui->tableView_DB->setModel(model);

        // Автоматичне налаштування розміру колонок
        // ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        // Робимо останню колонку розтягуваною
        // ui->tableView->horizontalHeader()->setStretchLastSection(true);

        // Дозволяємо користувачеві змінювати ширину колонок вручну
        ui->tableView_DB->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }
}

void UserWindow::loadHistory()
{
    qDebug() << "Username in UserWindow: history";

    QString errorMessage;
    QSqlTableModel *model = new QSqlTableModel();

    if (dbQueries.tableHistory(model, errorMessage)) {
        ui->tableView_history->setModel(model);

        // Автоматичне налаштування розміру колонок
        // ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        // Робимо останню колонку розтягуваною
        // ui->tableView->horizontalHeader()->setStretchLastSection(true);

        // Дозволяємо користувачеві змінювати ширину колонок вручну
        ui->tableView_DB->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }
}

void UserWindow::onReLoginButtonClicked() {
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    this->close();
}
