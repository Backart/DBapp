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

    QString errorMessage;
    QSqlTableModel *model = new QSqlTableModel(this, dbQueries.getDatabase());
    model->setTable("orders");  // Встановлюємо таблицю
    model->select();  // Завантажуємо дані

    // Підписуємося на сигнал dataChanged
    connect(model, &QSqlTableModel::dataChanged, this, &UserWindow::onDataChanged);
    qDebug() << "Connected dataChanged signal to onDataChanged slot.";

    ui->tableView_DB->setModel(model);
    ui->tableView_DB->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    // Зберігаємо зміни
    if (!model->submitAll()) {
        qDebug() << "Failed to save changes:" << model->lastError().text();
    } else {
        qDebug() << "Changes saved successfully!";
    }
}

void UserWindow::loadHistory()
{
    qDebug() << "Username in UserWindow: history";

    QString errorMessage;
    QSqlTableModel *model = new QSqlTableModel(this, dbQueries.getDatabase());
    model->setTable("orders");  // Встановлюємо таблицю
    model->select();  // Завантажуємо дані

    if (dbQueries.tableHistory(model, errorMessage)) {

        // Підписуємося на сигнал dataChanged
        connect(model, &QSqlTableModel::dataChanged, this, &UserWindow::onDataChanged);

        ui->tableView_history->setModel(model);

        // Автоматичне налаштування розміру колонок
        // ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        // Робимо останню колонку розтягуваною
        // ui->tableView->horizontalHeader()->setStretchLastSection(true);

        // Дозволяємо користувачеві змінювати ширину колонок вручну
        ui->tableView_DB->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }
}

void UserWindow::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);

    qDebug() << "Data changed detected!";

    QObject *senderObj = sender();
    if (!senderObj) {
        qDebug() << "Sender is null!";
        return;
    }

    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(senderObj);
    if (!model) {
        qDebug() << "Failed to cast sender to QSqlTableModel. Sender type:" << senderObj->metaObject()->className();
        return;
    }

    QVariant recordIdVar = model->data(model->index(topLeft.row(), 0));
    if (!recordIdVar.isValid() || recordIdVar.isNull()) {
        qDebug() << "Invalid Record ID!";
        return;
    }

    int recordId = recordIdVar.toInt();
    qDebug() << "Record ID:" << recordId;


    // Отримуємо старі дані
    QMap<QString, QVariant> oldData;
    for (int i = 0; i < model->columnCount(); ++i) {
        QString columnName = model->headerData(i, Qt::Horizontal).toString();
        oldData[columnName] = model->data(model->index(topLeft.row(), i), Qt::EditRole);
    }

    // Отримуємо нові дані
    QMap<QString, QVariant> newData;
    for (int i = 0; i < model->columnCount(); ++i) {
        QString columnName = model->headerData(i, Qt::Horizontal).toString();
        newData[columnName] = model->data(model->index(topLeft.row(), i));
    }

    qDebug() << "Old data:" << oldData;
    qDebug() << "New data:" << newData;

    // Логуємо зміни
    dbQueries.logHistory("UPDATE", "orders", recordId, oldData, newData);
}

void UserWindow::onReLoginButtonClicked() {
    if (mainWindow) {
        mainWindow->show();
    }
    this->close();
}
