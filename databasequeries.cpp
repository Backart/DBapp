#include "databasequeries.h"

DatabaseQueries::DatabaseQueries(QSqlDatabase& db) : database(db) {}

QSqlDatabase& DatabaseQueries::getDatabase() { return database; }

bool DatabaseQueries::authenticateUser(const QString& username, const QString& password, int& role_id, QString& errorMessage) {
    QSqlQuery query(database);
    query.prepare("SELECT password, role_id FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        ErrorMessages::showMessage(ErrorMessages::ERROR_500, query.lastError().text());
        return false;
    }

    if (!query.next()) {
        ErrorMessages::showMessage(ErrorMessages::ERROR_401, query.lastError().text());
        return false;
    }

    QString storedHash = query.value(0).toString();
    role_id = query.value(1).toInt();
    qDebug() << "Stored hash: " << storedHash;
    qDebug() << "Role ID: " << role_id;


    if (!SecurityUtils::verifyPassword(password, storedHash)) {
        ErrorMessages::showMessage(ErrorMessages::ERROR_401, query.lastError().text());
        return false;
    }

    return true;
}

bool DatabaseQueries::registerUser(const QString& username, const QString& password, const QString& confirmPassword, int& role_id, QString& errorMessage) {
    QSqlQuery query(database);
    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        ErrorMessages::showMessage(ErrorMessages::ERROR_400, query.lastError().text());
        return false;
    }

    if (password != confirmPassword) {
        ErrorMessages::showMessage(ErrorMessages::ERROR_401, query.lastError().text());
        return false;
    }

    QString hashedPassword = SecurityUtils::hashPassword(password); // Хешуємо пароль

    query.prepare("INSERT INTO users (username, password, role_id) VALUES (:username, :password, :role_id)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);
    query.bindValue(":role_id", role_id);

    if (!query.exec()) {
        ErrorMessages::showMessage(ErrorMessages::ERROR_201, query.lastError().text());
        return false;
    }

    ErrorMessages::showMessage(ErrorMessages::ERROR_200, query.lastError().text());
    return true;
}

bool DatabaseQueries::windowAdmin(QSqlQueryModel* model, QString& errorMessage){

    QSqlQuery query(database);
    query.prepare("SELECT * FROM users");

    if (!query.exec()) {
        ErrorMessages::showMessage(ErrorMessages::ERROR_400, query.lastError().text());
        return false;
    }
    model->setQuery(std::move(query));
    return true;
}

bool DatabaseQueries::windowUser(QSqlTableModel* model, QString& errorMessage) {
    model->setTable("orders");
    model->select(); // Виконує запит автоматично

    if (!model->submitAll()) {
        qDebug() << "Error saving changes:" << model->lastError().text();
        return false;
    }
    qDebug() << "Changes saved successfully!";

    if (model->lastError().isValid()) {
        errorMessage = model->lastError().text();
        ErrorMessages::showMessage(ErrorMessages::ERROR_400, errorMessage);
        return false;
    }

    return true;
}

bool DatabaseQueries::tableHistory(QSqlQueryModel* model, QString& errorMessage) {
    if (!database.isOpen()) {
        errorMessage = "Database is not open";
        return false;
    }

    QSqlQuery query(database);
    query.prepare("SELECT * FROM history");

    if (!query.exec()) {
        errorMessage = query.lastError().text();
        ErrorMessages::showMessage(ErrorMessages::ERROR_400, errorMessage);
        return false;
    }

    model->setQuery(query);
    return true;
}

void DatabaseQueries::logHistory(const QString &user, const QString &table) {
    if (!database.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    if (user.isEmpty() || table.isEmpty()) {
        qDebug() << "Error: user or table name is empty!";
        return;
    }

    qDebug() << "logHistory User:" << user;
    qDebug() << "logHistory Table:" << table;

    QSqlQuery query(database);
    query.prepare(R"(
        INSERT INTO history (user_name, table_name)
        VALUES (?, ?)
    )");

    query.addBindValue(user);
    query.addBindValue(table);

    qDebug() << "Executing query:" << query.lastQuery();
    qDebug() << "With values: user =" << user << ", table =" << table;

    if (!query.exec()) {
        qDebug() << "Error inserting history log:" << query.lastError().text();
        qDebug() << "Query:" << query.lastQuery();
        database.rollback(); // Відкат транзакції у разі помилки
    } else {
        database.commit(); // Підтвердження транзакції
        qDebug() << "History log inserted successfully!";
    }
}
