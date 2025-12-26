#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QLibraryInfo>
#include <QStandardPaths>
#include <QPluginLoader>
#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "Starting SQL Database Test";
    qDebug() << "Qt Version:" << QT_VERSION_STR;
    qDebug() << "Runtime Qt Version:" << qVersion();
    
    // CRITICAL: Add plugin paths BEFORE any database operations
    QString qtDir = qgetenv("QTDIR");
    if (!qtDir.isEmpty()) {
        QString pluginPath = qtDir + "/plugins";
        QString sqlDriverPath = pluginPath + "/sqldrivers";
        
        qDebug() << "Adding plugin paths:";
        qDebug() << "  - Main plugins:" << pluginPath;
        qDebug() << "  - SQL drivers:" << sqlDriverPath;
        
        app.addLibraryPath(pluginPath);
        app.addLibraryPath(sqlDriverPath);
        
        qDebug() << "Updated library paths:" << app.libraryPaths();
        
        // AIX SPECIFIC: Enhanced plugin loading with dependency check
        QString pluginFile = sqlDriverPath + "/libqsqlite.so";
        qDebug() << "Attempting manual plugin load:" << pluginFile;
        
        if (QFile::exists(pluginFile)) {
            QPluginLoader loader(pluginFile);
            qDebug() << "Plugin file exists, checking metadata...";
            qDebug() << "Plugin metadata:" << loader.metaData();
            
            // Force resolution of dependencies
            loader.setLoadHints(QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint);
            
            if (loader.load()) {
                qDebug() << "Plugin loaded successfully";
                QObject *plugin = loader.instance();
                if (plugin) {
                    qDebug() << "Plugin instance created successfully";
                    qDebug() << "Plugin class name:" << plugin->metaObject()->className();
                } else {
                    qDebug() << "Failed to create plugin instance:" << loader.errorString();
                }
            } else {
                qDebug() << "Failed to load plugin:" << loader.errorString();
                qDebug() << "Possible solutions:";
                qDebug() << "1. Check library dependencies with ldd";
                qDebug() << "2. Ensure all Qt libraries are in the same directory";
                qDebug() << "3. Create symbolic links for library compatibility";
            }
        } else {
            qDebug() << "Plugin file does not exist:" << pluginFile;
        }
    }
    
    // Force refresh of available drivers
    qDebug() << "Available SQL drivers:" << QSqlDatabase::drivers();
    
    // If QSQLITE is not available, provide detailed error information
    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        qCritical() << "QSQLITE driver not found!";
        qDebug() << "This usually means:";
        qDebug() << "1. Plugin has dependency path issues";
        qDebug() << "2. Plugin is not compatible with current Qt libraries";
        qDebug() << "3. Plugin failed to load due to missing dependencies";
        return 1;
    }
    
    // Test database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test.db");
    
    if (!db.open()) {
        qCritical() << "Failed to open database:" << db.lastError().text();
        qDebug() << "Database error details:";
        qDebug() << "  Type:" << db.lastError().type();
        qDebug() << "  Driver text:" << db.lastError().driverText();
        qDebug() << "  Database text:" << db.lastError().databaseText();
        // Removed deprecated number() call
        return 1;
    }
    
    qDebug() << "Database opened successfully";
    
    // Create table
    QSqlQuery query;
    QString createTable = "CREATE TABLE IF NOT EXISTS users ("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "name TEXT NOT NULL, "
                         "email TEXT UNIQUE, "
                         "age INTEGER"
                         ")";
    
    if (!query.exec(createTable)) {
        qCritical() << "Failed to create table:" << query.lastError().text();
        return 1;
    }
    
    qDebug() << "Table created successfully";
    
    // Insert data
    QString insertData = "INSERT INTO users (name, email, age) VALUES "
                        "('John Doe', 'john@example.com', 30), "
                        "('Jane Smith', 'jane@example.com', 25), "
                        "('Bob Johnson', 'bob@example.com', 35)";
    
    if (!query.exec(insertData)) {
        qDebug() << "Insert failed (might be duplicate):" << query.lastError().text();
    } else {
        qDebug() << "Data inserted successfully";
    }
    
    // Query data
    if (!query.exec("SELECT * FROM users")) {
        qCritical() << "Failed to query data:" << query.lastError().text();
        return 1;
    }
    
    qDebug() << "Query executed successfully";
    qDebug() << "Users in database:";
    
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString email = query.value(2).toString();
        int age = query.value(3).toInt();
        
        qDebug() << "ID:" << id << "Name:" << name << "Email:" << email << "Age:" << age;
    }
    
    // Test prepared statement
    QSqlQuery prepared;
    prepared.prepare("SELECT * FROM users WHERE age > ?");
    prepared.addBindValue(28);
    
    if (!prepared.exec()) {
        qCritical() << "Prepared statement failed:" << prepared.lastError().text();
        return 1;
    }
    
    qDebug() << "Prepared statement executed successfully";
    qDebug() << "Users older than 28:";
    
    while (prepared.next()) {
        QString name = prepared.value(1).toString();
        int age = prepared.value(3).toInt();
        qDebug() << "Name:" << name << "Age:" << age;
    }
    
    db.close();
    qDebug() << "Database closed";
    
    // Clean up
    QDir::current().remove("test.db");
    qDebug() << "Test completed successfully";
    
    return 0;
}