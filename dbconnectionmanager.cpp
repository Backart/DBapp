#include "dbconnectionmanager.h"
#include <QPluginLoader>

#include <QLibraryInfo>
#include <QProcessEnvironment>
#include <QPluginLoader>

void printQtEnvVariables() {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QStringList keys = env.keys();

    qDebug() << "==================== Qt Environment Variables ====================";
    for (const QString &key : keys) {
        if (key.startsWith("QT") || key.startsWith("LD") || key.contains("PATH", Qt::CaseInsensitive)) {
            qDebug() << key << "=" << env.value(key);
        }
    }
}

void checkPostgreSQLDriverDependencies() {
    QString driverPath = "/usr/lib/qt6/plugins/sqldrivers/libqsqlpsql.so";
    qDebug() << "Checking dependencies for:" << driverPath;

    QProcess process;
    process.start("ldd", QStringList() << driverPath);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    QString errorOutput = process.readAllStandardError();

    qDebug() << "=== ldd Output ===";
    qDebug().noquote() << output;

    if (!errorOutput.isEmpty()) {
        qDebug() << "=== ldd Errors ===";
        qDebug().noquote() << errorOutput;
    }
}


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



    qputenv("QT_DEBUG_PLUGINS", QByteArray("1"));
    printQtEnvVariables();
    checkPostgreSQLDriverDependencies();

    qDebug() << "Available SQL Drivers: " << QSqlDatabase::drivers();

    QDir pluginsDir("/usr/lib/qt6/plugins/sqldrivers");
    QStringList files = pluginsDir.entryList(QDir::Files);

    qDebug() << "SQL driver plugins found in /usr/lib/qt6/plugins/sqldrivers:";
    for (const QString &file : files) {
        qDebug() << file;

        QPluginLoader loader(pluginsDir.absoluteFilePath(file));
        QObject *plugin = loader.instance();
        if (plugin) {
            qDebug() << "Loaded plugin: " << file;
        } else {
            qDebug() << "Failed to load plugin: " << file << "Error:" << loader.errorString();
        }
    }


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
