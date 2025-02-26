#include "dbconnectionmanager.h"

DBConnectionManager::DBConnectionManager() {
    // якщо буду робити окреме вікно для конфігурації
    /*QString configDirPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QString configFilePath = configDirPath + "/config/dataConnectConfig.ini";
    if (!QFile::exists(configFilePath)) {
        qDebug() << "Config file does not exist at:" << configFilePath;
        return;
    }
    qDebug() << "Config file exists at:" << configFilePath;
    QSettings settings(configFilePath, QSettings::IniFormat);
    */

    //старе підключення
    /* db = QSqlDatabase::addDatabase("QPSQL");
     db.setDatabaseName("TestDB");
     db.setUserName("postgres");
     db.setPassword("1");
    */

    //костильний варіант підключення
    /*
    QDir configDir(QCoreApplication::applicationDirPath());
    configDir.cdUp();  // Піднімаємося на рівень вище
    configDir.cdUp();  // Піднімаємося на рівень вище
    configDir.cd("config");

    QString configFilePath = configDir.filePath("dataConnectConfig.ini");

    qDebug() << "Config file exists at:" << configFilePath;
    QSettings settings(configFilePath, QSettings::IniFormat);
    */


    // Отримуємо шлях до папки проєкту
    QString projectDir = QCoreApplication::applicationDirPath() + "/../../config";
    QString configPath = QDir::cleanPath(projectDir + "/dataConnectConfig.ini");

    QSettings settings(configPath, QSettings::IniFormat);

    QString host = settings.value("Database/host", "127.0.0.1").toString();
    int port = settings.value("Database/port", 5432).toInt();
    QString dbName = settings.value("Database/name", "").toString();
    QString user = settings.value("Database/user", "postgres").toString();
    QString password = settings.value("Database/password", "").toString();

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);

}

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
