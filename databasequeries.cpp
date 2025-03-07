#include "databasequeries.h"

DatabaseQueries::DatabaseQueries(QSqlDatabase& db) : database(db) {}

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

bool DatabaseQueries::tableHistory(QSqlQueryModel* model, QString& errorMessage){
    QSqlQuery query(database);
    query.prepare("SELECT * FROM history");

    if (!query.exec()) {
        ErrorMessages::showMessage(ErrorMessages::ERROR_400, query.lastError().text());
        return false;
    }
    model->setQuery(std::move(query));
    return true;
}

void logHistory(const QString &action, const QString &table, int recordId,
                const QMap<QString, QVariant> &oldData, const QMap<QString, QVariant> &newData) {
    // Виключаємо системні таблиці
    static const QSet<QString> excludedTables = {"role", "users", "history"};
    if (excludedTables.contains(table)) return;

    // Створення SQL-запиту для вставки в таблицю історії
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO history (action_type, table_name, record_id, user_name, timestamp, changes)
        VALUES (?, ?, ?, CURRENT_USER, NOW(), ?)
    )");

    QJsonObject changes;  // Об'єкт для збереження змін
    // Порівнюємо старі та нові дані
    for (auto it = oldData.constBegin(); it != oldData.constEnd(); ++it) {
        const QString &key = it.key();
        if (oldData.value(key) != newData.value(key)) {
            changes[key] = QString("Changed from '%1' to '%2'")
            .arg(oldData.value(key).toString(), newData.value(key).toString());
        }
    }

    // Додаємо значення в запит
    query.addBindValue(action);
    query.addBindValue(table);
    query.addBindValue(recordId);
    query.addBindValue(QString(QJsonDocument(changes).toJson(QJsonDocument::Compact)));

    // Виконання запиту
    if (!query.exec()) {
        qDebug() << "Error inserting history log:" << query.lastError().text();
    }
}


/*void logHistory(const QString &action, const QString &table, int recordId,
                const QMap<QString, QVariant> &oldData, const QMap<QString, QVariant> &newData) {

    // Виключаємо системні таблиці
    static const QSet<QString> excludedTables = {"role", "users", "history"};
    if (excludedTables.contains(table)) return;

    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO history (action_type, table_name, record_id, user_name, timestamp, changes)
        VALUES (?, ?, ?, CURRENT_USER, NOW(), ?)
    )");

    QJsonObject changes;
    for (auto it = oldData.constBegin(); it != oldData.constEnd(); ++it) {
        const QString &key = it.key();
        if (oldData.value(key) != newData.value(key)) {
            changes[key] = QString("Changed from '%1' to '%2'")
            .arg(oldData.value(key).toString(), newData.value(key).toString());
        }
    }

    query.addBindValue(action);
    query.addBindValue(table);
    query.addBindValue(recordId);
    query.addBindValue(QString(QJsonDocument(changes).toJson(QJsonDocument::Compact)));

    if (!query.exec()) {
        qDebug() << "Error inserting history log:" << query.lastError().text();
    }
}*/
