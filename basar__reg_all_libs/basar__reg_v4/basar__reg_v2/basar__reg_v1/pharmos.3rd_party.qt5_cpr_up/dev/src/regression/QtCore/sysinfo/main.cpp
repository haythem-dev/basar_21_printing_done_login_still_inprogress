#include <QCoreApplication>
#include <QSysInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "System information:";
    qDebug() << "  Machine host name:" << QSysInfo::machineHostName();
    qDebug() << "  Machine unique id:" << QSysInfo::buildAbi();
    qDebug() << "  Operating system:" << QSysInfo::prettyProductName();
    qDebug() << "  OS Type:" << QSysInfo::productType();
    qDebug() << "  OS Version:" << QSysInfo::productVersion();
    qDebug() << "  Kernel Type:" << QSysInfo::kernelType();
    qDebug() << "  Kernel Version:" << QSysInfo::kernelVersion();
    qDebug() << "  CPU Architecture:" << QSysInfo::currentCpuArchitecture();

    return 0;
}
