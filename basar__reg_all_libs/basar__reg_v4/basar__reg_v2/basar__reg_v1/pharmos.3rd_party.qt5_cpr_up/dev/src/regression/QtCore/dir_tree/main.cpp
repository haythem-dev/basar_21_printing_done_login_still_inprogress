#include <QCoreApplication>
#include <QDir>
#include <QDebug>

void printDirectory(const QString &path, int indentLevel = 0)
{
    QDir dir(path);
    QStringList entries = dir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

    foreach(const QString &entry, entries) {
        QString fullPath = dir.filePath(entry);
        QString indent(indentLevel * 2, ' ');
        qDebug() << indent << entry;

        if (QDir(fullPath).exists() && QDir(fullPath).entryList(QDir::Dirs | QDir::NoDotAndDotDot).count() > 0) {
            // Recurse into subdirectory
            printDirectory(fullPath, indentLevel + 1);
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString startPath = QDir::homePath(); // Or specify your directory
    if (argc > 1) {
        startPath = QString(argv[1]);
    }

    qDebug() << "Directory tree for:" << startPath;
    printDirectory(startPath);

    return 0;
}
