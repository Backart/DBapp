#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(QSqlDatabase& db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
    ,dbQueries(db)
{
    ui->setupUi(this);

    loadUserOrders();
}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::loadUserOrders()
{
    qDebug() << "Username in UserWindow: user";

    QString errorMessage;
    QSqlQueryModel *model = new QSqlQueryModel();

    if (dbQueries.windowUser(model, errorMessage)) {
        ui->tableView->setModel(model);

        // Автоматичне налаштування розміру колонок
        // ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        // Робимо останню колонку розтягуваною
        // ui->tableView->horizontalHeader()->setStretchLastSection(true);

        // Дозволяємо користувачеві змінювати ширину колонок вручну
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }
}
