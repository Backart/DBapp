#include "databasequeries.h"

DatabaseQueries::DatabaseQueries(QSqlDatabase& db) : database(db) {}

QString hashPassword(const QString& password) {
    return QString::fromStdString(bcryptcpp::generateHash(password.toStdString()));
}

bool verifyPassword(const QString& password, const QString& hash) {
    return bcryptcpp::validatePassword(password.toStdString(), hash.toStdString());
}

bool DatabaseQueries::authenticateUser(const QString& username, const QString& password, int& role_id, QString& errorMessage) {
    QSqlQuery query(database);
    query.prepare("SELECT password, role_id FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        errorMessage = "Failed to execute query: " + query.lastError().text();
        ErrorMessages::showCritical(ErrorMessages::ERROR_500, errorMessage);
        return false;
    }

    if (!query.next()) {
        errorMessage = "Invalid username or password.";
        ErrorMessages::showWarning(ErrorMessages::ERROR_401, errorMessage);
        return false;
    }

    QString storedHash = query.value(0).toString();
    role_id = query.value(1).toInt();

    qDebug() << "Stored hash: " << storedHash;
    qDebug() << "Role ID: " << role_id;


    if (!verifyPassword(password, storedHash)) {
        errorMessage = "Invalid username or password.";
        ErrorMessages::showWarning(ErrorMessages::ERROR_401, errorMessage);
        return false;
    }

    return true;
}

bool DatabaseQueries::registerUser(const QString& username, const QString& password, const QString& confirmPassword, int& role_id, QString& errorMessage) {
    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QString errorMessage = "All fields must be filled!";
        ErrorMessages::showWarning(ErrorMessages::ERROR_400, errorMessage);
        return false;
    }

    if (password != confirmPassword) {
        QString errorMessage = "Passwords do not match!";
        ErrorMessages::showWarning(ErrorMessages::ERROR_401, errorMessage);
        return false;
    }

    QString hashedPassword = hashPassword(password); // Хешуємо пароль

    QSqlQuery query(database);
    query.prepare("INSERT INTO users (username, password, role_id) VALUES (:username, :password, :role_id)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);
    query.bindValue(":role_id", role_id);

    if (!query.exec()) {
        errorMessage = "Failed to register user: " + query.lastError().text();
        ErrorMessages::showCritical(ErrorMessages::ERROR_500, errorMessage);
        return false;
    }

    return true;
}

bool DatabaseQueries::windowAdmin(QSqlQueryModel* model, QString& errorMessage){

    QSqlQuery query(database);
    query.prepare("SELECT * FROM users");

    if (!query.exec()) {
        errorMessage = "Database Error" + query.lastError().text();
        return false;
    }
    model->setQuery(std::move(query));
    return true;
}

bool DatabaseQueries::windowUser(QSqlQueryModel* model, QString& errorMessage){

    QSqlQuery query(database);
    query.prepare("SELECT * FROM orders");

    if (!query.exec()) {
        errorMessage = "Database Error" + query.lastError().text();
        return false;
    }
    model->setQuery(std::move(query));
    return true;
}
