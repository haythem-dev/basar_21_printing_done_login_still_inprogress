#include <QCoreApplication>
#include <QFileSystemWatcher>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QStringList>

// Function to list directory contents (simulate processing)
void listDirectoryContents(const QString &path) {
    QDir dir(path);
    QStringList entries = dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
    qDebug() << "Contents of" << path << ":";
    for (const QString &entry : entries) {
        qDebug() << "  -" << entry;
    }
    qDebug() << ""; // newline for clarity
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream out(stdout);

    QStringList dirPaths;

    // Collect directory paths from command line args
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            QString path = QString::fromLocal8Bit(argv[i]);
            QDir dir(path);
            if (dir.exists()) {
                dirPaths << path;
            } else {
                qWarning() << "Warning: Directory does not exist:" << path;
            }
        }
    } else {
        // No args: default to current directory
        dirPaths << QDir::currentPath();
    }

    // List directories in parallel using QtConcurrent
    QList<QFuture<void>> futures;
    for (const QString &path : dirPaths) {
        futures.append(QtConcurrent::run(listDirectoryContents, path));
    }

    // Wait for all parallel tasks to finish
    for (QFuture<void> &future : futures) {
        future.waitForFinished();
    }

    // Set up the QFileSystemWatcher
    QFileSystemWatcher watcher;
    for (const QString &path : dirPaths) {
        watcher.addPath(path);
    }

    out << "Monitoring directories for changes. Press Ctrl+C to exit.\n";

    // Connect signals
    QObject::connect(&watcher, &QFileSystemWatcher::directoryChanged,
                     [](const QString &path){
                         qDebug() << "Directory changed:" << path;
                     });
    QObject::connect(&watcher, &QFileSystemWatcher::fileChanged,
                     [](const QString &path){
                         qDebug() << "File changed:" << path;
                     });

    return app.exec();
}
