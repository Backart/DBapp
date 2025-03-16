#include "dbconnectionmanager.h"

DBConnectionManager::DBConnectionManager() {}

DBConnectionManager::~DBConnectionManager() {
    if (db.isOpen()) {
        db.close();
    }
}

DBConnectionManager& DBConnectionManager::instance() {
    static DBConnectionManager instance;
    return instance;
}

QSqlDatabase DBConnectionManager::getDatabase() {
    return db;
}

bool DBConnectionManager::openDatabase() {
    if (db.isOpen()) {
        db.close();
    }

    // Видалити існуюче підключення (важливо!)
    QString connectionName = db.connectionName();
    db = QSqlDatabase(); // Очистити поточний об'єкт
    QSqlDatabase::removeDatabase(connectionName); // Видалити попереднє з'єднання

    // Створюємо нове підключення заново
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    configPath = QDir::cleanPath(configPath + "/dataConnectConfig.ini");
    QSettings settings(configPath, QSettings::IniFormat);

    QString host = settings.value("Database/host").toString();
    int port = settings.value("Database/port").toInt();
    QString dbName = settings.value("Database/name").toString();
    QString user = settings.value("Database/user").toString();
    QString password = settings.value("Database/password").toString();

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);

    //qDebug() << "Config file path:" << configPath;

    if (!db.open()) {
        qDebug() << "Database not connected: " << db.lastError().text();
        return false;
    }
    qDebug() << "Database connected";
    return true;
}

void DBConnectionManager::closeDatabase() {
    if (db.isOpen()) {
        db.close();
    }
}
