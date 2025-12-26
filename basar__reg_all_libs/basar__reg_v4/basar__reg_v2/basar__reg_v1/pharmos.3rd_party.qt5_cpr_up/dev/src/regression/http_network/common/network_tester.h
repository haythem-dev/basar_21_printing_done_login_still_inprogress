#ifndef NETWORK_TESTER_H
#define NETWORK_TESTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QCoreApplication>
#include <QSysInfo>

class NetworkTester : public QObject
{
    Q_OBJECT

public:
    explicit NetworkTester(QObject *parent = nullptr);
    void setSSLEnabled(bool enabled) { m_sslEnabled = enabled; }
    bool isSSLEnabled() const { return m_sslEnabled; }
    void setBaseUrl(const QString &url) { m_baseUrl = url; }
    QString baseUrl() const { return m_baseUrl; }

public slots:
    void startTests();

private slots:
    void onRequestFinished(QNetworkReply *reply);
    void timeout();

private:
    QNetworkAccessManager *m_manager;
    int m_completedTests;
    int m_totalTests;
    bool m_sslEnabled;
    QString m_baseUrl;
    
    void runTest(const QString &testName, const QNetworkRequest &request, 
                 const QByteArray &data = QByteArray(), 
                 const QString &method = "GET");
    void printTestResult(const QString &testName, QNetworkReply *reply);
    void finishTesting();
};

#endif // NETWORK_TESTER_H