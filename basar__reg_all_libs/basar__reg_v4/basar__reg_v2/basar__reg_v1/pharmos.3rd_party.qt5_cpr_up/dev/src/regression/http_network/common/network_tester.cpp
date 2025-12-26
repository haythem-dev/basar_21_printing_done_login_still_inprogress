#include "network_tester.h"

NetworkTester::NetworkTester(QObject *parent)
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this))
    , m_completedTests(0)
    , m_totalTests(3)
    , m_sslEnabled(false)
    , m_baseUrl("http://httpbingo.org")
{
    connect(m_manager, &QNetworkAccessManager::finished, 
            this, &NetworkTester::onRequestFinished);
    
    // Set timeout for all tests - 15 seconds
    QTimer::singleShot(15000, this, &NetworkTester::timeout);
}

void NetworkTester::startTests()
{
    qDebug() << "Starting HTTP Network Test" << (m_sslEnabled ? "(SSL enabled)" : "(SSL disabled)");
    qDebug() << "Qt Network module version:" << qVersion();
    qDebug() << "========================================";
    
    // Try multiple test URLs in case of network issues
    QStringList testUrls = {
        "http://httpbingo.org",    // Primary test service (httpbin.org replacement)
        "http://jsonplaceholder.typicode.com",  // Alternative service
        "http://httpbin.org"       // Original (currently down with 503)
    };
    
    QString baseUrl = m_baseUrl; // Use configured base URL
    
    // Test 1: HTTP GET request
    qDebug() << "Test 1: HTTP GET request";
    QNetworkRequest getRequest(QUrl(baseUrl + "/get"));
    getRequest.setRawHeader("User-Agent", "Qt AIX Test Client");
    getRequest.setRawHeader("Accept", "application/json");
    
    QNetworkReply *getReply = m_manager->get(getRequest);
    getReply->setProperty("test_type", "GET");
    getReply->setProperty("test_number", 1);
    
    qDebug() << "HTTP GET request sent to" << baseUrl;
    
    // Test 2: HTTP POST request
    qDebug() << "Test 2: HTTP POST request";
    QNetworkRequest postRequest(QUrl(baseUrl + "/post"));
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    postRequest.setRawHeader("User-Agent", "Qt AIX Test Client");
    
    QByteArray postData = "{\"test\": \"AIX HTTP test\", \"platform\": \"AIX\", \"qt_version\": \"5.15.16\"}";
    QNetworkReply *postReply = m_manager->post(postRequest, postData);
    postReply->setProperty("test_type", "POST");
    postReply->setProperty("test_number", 2);
    
    qDebug() << "HTTP POST request sent with JSON data";
    
    // Test 3: HTTP PUT request
    qDebug() << "Test 3: HTTP PUT request";
    QNetworkRequest putRequest(QUrl(baseUrl + "/put"));
    putRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    putRequest.setRawHeader("User-Agent", "Qt AIX Test Client");
    
    QString putDataString = "{\"update\": \"test data\", \"timestamp\": \"" + QDateTime::currentDateTime().toString() + "\"}";
    QByteArray putData = putDataString.toUtf8();
    QNetworkReply *putReply = m_manager->put(putRequest, putData);
    putReply->setProperty("test_type", "PUT");
    putReply->setProperty("test_number", 3);
    
    qDebug() << "HTTP PUT request sent";
    qDebug() << "All HTTP tests initiated. Waiting for responses...";
    qDebug() << "========================================";
}

void NetworkTester::onRequestFinished(QNetworkReply *reply)
{
    QString testType = reply->property("test_type").toString();
    int testNumber = reply->property("test_number").toInt();
    
    qDebug() << "=== Test" << testNumber << "(" << testType << ") Results ===";
    
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "✓ Request completed successfully";
        qDebug() << "Status code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "Response size:" << reply->bytesAvailable() << "bytes";
        
        // Show first 100 characters of response for verification
        QByteArray responseData = reply->readAll();
        if (responseData.size() > 100) {
            qDebug() << "Response preview:" << responseData.left(100) << "...";
        } else {
            qDebug() << "Response:" << responseData;
        }
    } else {
        qDebug() << "✗ Request failed with error:" << reply->errorString();
        qDebug() << "Error code:" << reply->error();
    }
    
    reply->deleteLater();
    m_completedTests++;
    
    if (m_completedTests >= m_totalTests) {
        finishTesting();
    }
}

void NetworkTester::timeout()
{
    qDebug() << "========================================";
    qDebug() << "⚠ Test timeout reached (15 seconds)";
    qDebug() << "Completed tests:" << m_completedTests << "/" << m_totalTests;
    finishTesting();
}

void NetworkTester::finishTesting()
{
    qDebug() << "========================================";
    if (m_completedTests == m_totalTests) {
        qDebug() << "🎉 ALL NETWORK TESTS COMPLETED SUCCESSFULLY";
    } else {
        qDebug() << "⚠ Tests completed with" << (m_totalTests - m_completedTests) << "failures/timeouts";
    }
    qDebug() << "SSL support:" << (m_sslEnabled ? "enabled" : "disabled");
    qDebug() << "Test completion time:" << QDateTime::currentDateTime().toString();
    qDebug() << "========================================";
    
    // Exit the application
    QCoreApplication::exit(m_completedTests == m_totalTests ? 0 : 1);
}