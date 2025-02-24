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

    if (dbQueries.windowUser(model, errorMessage))
        ui->tableView->setModel(model);

}

