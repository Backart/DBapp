#include "adminwindow.h"
#include "ui_adminwindow.h"

AdminWindow::AdminWindow(QSqlDatabase& db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow)
    ,dbQueries(db)
{
    ui->setupUi(this);

    loadAdminTables();
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::loadAdminTables()
{
    qDebug() << "Username in UserWindow: admin";

    QString errorMessage;
    QSqlQueryModel *model = new QSqlQueryModel();

    if (dbQueries.windowAdmin(model, errorMessage))
        ui->tableView->setModel(model);

}
