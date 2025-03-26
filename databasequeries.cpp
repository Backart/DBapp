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

bool DatabaseQueries::updateUsername(const QString& oldUsername, const QString& newUsername) {
    QSqlQuery query(database);
    query.prepare("UPDATE users SET username = :newUsername WHERE username = :oldUsername");
    query.bindValue(":oldUsername", oldUsername);
    query.bindValue(":newUsername", newUsername);

    if (!query.exec()) {
        ErrorMessages::showMessage(ErrorMessages::ERROR_500, query.lastError().text());
        return false;
    }
    return true;
}

bool DatabaseQueries::updatePassword(const QString& username, const QString& newPassword) {
    QSqlQuery query(database);
    query.prepare("UPDATE users SET password = :newPassword WHERE username = :username");
    query.bindValue(":username", username);
    query.bindValue(":newPassword", newPassword);

    if (!query.exec()) {
        ErrorMessages::showMessage(ErrorMessages::ERROR_500, query.lastError().text());
        return false;
    }
    return true;
}

bool DatabaseQueries::isUsernameExist(const QString& username) {
    QSqlQuery query(database);
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        ErrorMessages::showMessage(ErrorMessages::ERROR_500, query.lastError().text());
        return false;
    }

    // Якщо результат запиту більше 0, то такий користувач існує
    query.next();
    return query.value(0).toInt() > 0;
}

