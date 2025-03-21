#include "userwindow.h"
#include "mainwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(QSqlDatabase& db, MainWindow *mainWindow, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
    , dbQueries(db)
    , mainWindow(mainWindow)
    , proxyModel(new QSortFilterProxyModel(this))
{
    ui->setupUi(this);

    loadHistory();
    loadDatabaseTables();
    onReLoginButtonClicked();

    connect(ui->pushButton_add_row, &QPushButton::clicked, this, &UserWindow::addRow);
    connect(ui->pushButton_re_login_singup, &QPushButton::clicked, this, &UserWindow::onReLoginButtonClicked);
    connect(ui->treeView_tableDB, &QTreeView::clicked, this, &UserWindow::onTableSelected);
    connect(ui->textEdit_search, &QTextEdit::textChanged, this, &UserWindow::onSearchText);

}

UserWindow::~UserWindow()
{
    delete ui;
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
    model->select();  // Завантажуємо дані з таблиці history

    ui->tableView_history->setModel(model);
    ui->tableView_history->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    if (model->lastError().isValid()) {
        errorMessage = model->lastError().text();
        qDebug() << "Error loading history:" << errorMessage;
    } else {
        qDebug() << "History loaded successfully!";
    }

    // Перевірка, чи база даних відкрита
    if (!dbQueries.getDatabase().isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    // Якщо змінились дані в базі (не в таблиці history)
    if (dbQueries.getDatabase().transaction()) {
        // Запит на зміну даних в базі
        // Якщо виконано зміну, додаємо запис в історію

        QString currentUser = mainWindow->getUsername();
        QSqlQuery query(dbQueries.getDatabase());

        query.prepare(R"(
            INSERT INTO history (user_name)
            VALUES (?)
        )");

        query.addBindValue(currentUser);  // Використовуємо currentUser

        if (!query.exec()) {
            qDebug() << "Error inserting history log:" << query.lastError().text();
            dbQueries.getDatabase().rollback();  // Відкат транзакції у разі помилки
        } else {
            dbQueries.getDatabase().commit();  // Підтвердження транзакції
            qDebug() << "History log inserted successfully!";
        }
    }
}

void UserWindow::loadDatabaseTables() {
    // Перевіряємо, чи база даних відкрита
    if (!dbQueries.getDatabase().isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    // Отримуємо список таблиць з бази даних
    QStringList tables = dbQueries.getDatabase().tables();
    qDebug() << "Tables in database:" << tables;

    // Список таблиць, які потрібно приховати
    QStringList hiddenTables = {"role", "status", "history"};

    // Створюємо модель для treeView
    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList() << "Tables");

    // Додаємо таблиці, які не входять до списку hiddenTables
    for (const QString &tableName : tables) {
        if (!hiddenTables.contains(tableName)) {
            QStandardItem *item = new QStandardItem(tableName);
            model->appendRow(item);
        }
    }

    // Встановлюємо модель для treeView
    ui->treeView_tableDB->setModel(model);
}

void UserWindow::onTableSelected(const QModelIndex &index) {
    QString tableName = index.data().toString();
    qDebug() << "Selected table:" << tableName;

    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable(tableName);
    model->select();

    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);  // Пошук без урахування регістру
    proxyModel->setFilterKeyColumn(-1);  // Шукає по всіх колонках

    ui->tableView_DB->setModel(proxyModel);
    ui->tableView_DB->setSortingEnabled(true);  // Дозволяємо сортування
    ui->tableView_DB->horizontalHeader()->setSortIndicatorShown(true);  // Показуємо іконку сортування
    ui->tableView_DB->horizontalHeader()->setSectionsClickable(true);  // Дозволяємо клікати заголовки
}

void UserWindow::addRow() {
    qDebug() << "Adding new row to table";

    if (!ui->tableView_DB->model()) {
        qDebug() << "No model assigned to tableView_DB!";
        return;
    }

    // Перетворюємо модель до QSqlTableModel
    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(ui->tableView_DB->model());
    if (!model) {
        qDebug() << "Failed to cast model to QSqlTableModel";
        return;
    }

    // Вставляємо новий рядок
    int row = model->rowCount();
    model->insertRow(row);

    // Відображаємо зміну
    ui->tableView_DB->scrollToBottom();

    qDebug() << "New row added at index:" << row;
}

void UserWindow::onSearchText() {
    QString searchText = ui->textEdit_search->toPlainText();
    qDebug() << "Search query:" << searchText;

    proxyModel->setFilterFixedString(searchText); // Фільтр по всіх колонках
}

