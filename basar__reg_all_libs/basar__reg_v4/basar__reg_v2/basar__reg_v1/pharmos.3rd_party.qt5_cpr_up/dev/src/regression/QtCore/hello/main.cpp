#include <QCoreApplication>
#include <QString>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream out(stdout);
    out << "Hello, Qt5 from the terminal!\n";
    return 0;
}
