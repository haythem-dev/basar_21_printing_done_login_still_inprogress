#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QSysInfo>
#include "../common/network_tester.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "Qt HTTP Network Regression Test for AIX (Local Server)";
    qDebug() << "Version: 1.0 (SSL Disabled - Local Testing)";
    qDebug() << "Platform:" << QSysInfo::prettyProductName();
    qDebug() << "Qt Version:" << qVersion();
    qDebug() << "Build Date:" << __DATE__ << __TIME__;
    qDebug() << "========================================";
    
    NetworkTester tester;
    tester.setSSLEnabled(false);
    tester.setBaseUrl("http://localhost:8080");  // Use local server
    
    // Start tests after 100ms delay
    QTimer::singleShot(100, &tester, &NetworkTester::startTests);
    
    return app.exec();
}