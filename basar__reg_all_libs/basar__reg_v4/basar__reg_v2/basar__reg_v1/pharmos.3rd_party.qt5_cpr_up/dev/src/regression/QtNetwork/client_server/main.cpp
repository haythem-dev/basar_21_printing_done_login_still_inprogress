#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

// Server class
class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr) : QObject(parent) {
        server = new QTcpServer(this);
        connect(server, &QTcpServer::newConnection, this, &MyServer::handleNewConnection);

        const quint16 port = 12345;
        if (!server->listen(QHostAddress::LocalHost, port)) {
            qCritical() << "Server failed to start:" << server->errorString();
            QCoreApplication::exit(1);
        } else {
            qDebug() << "Server listening on port" << port;
        }

        // Schedule shutdown after 5 seconds
        QTimer::singleShot(5000, this, &MyServer::shutdown);
    }

private slots:
    void handleNewConnection() {
        QTcpSocket* socket = server->nextPendingConnection();
        qDebug() << "[Server] Client from" << socket->peerAddress().toString();

        connect(socket, &QTcpSocket::readyRead, [=]() {
            QByteArray data = socket->readAll();
            qDebug() << "[Server] Received:" << data;
            // reply
            socket->write("Server reply: " + data);
        });
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    }

    void shutdown() {
        qDebug() << "[Server] Shutting down after 5 seconds.";
        server->close();
        QCoreApplication::quit();
    }

private:
    QTcpServer *server;
};

// Client class
class MyClient : public QObject
{
    Q_OBJECT
public:
    explicit MyClient(QObject *parent = nullptr) : QObject(parent) {
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::connected, this, &MyClient::onConnected);
        connect(socket, &QTcpSocket::readyRead, this, &MyClient::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &MyClient::onDisconnected);
        connect(socket, &QTcpSocket::errorOccurred, [](QAbstractSocket::SocketError err){
            qWarning() << "[Client] Error:" << err;
        });

        socket->connectToHost("127.0.0.1", 12345);
    }

private slots:
    void onConnected() {
        qDebug() << "[Client] Connected.";
        socket->write("Hello from client");
    }

    void onReadyRead() {
        QByteArray data = socket->readAll();
        qDebug() << "[Client] Received:" << data;
        socket->disconnectFromHost(); // finish after receiving reply
    }

    void onDisconnected() {
        qDebug() << "[Client] Disconnected.";
        QCoreApplication::quit();
    }

private:
    QTcpSocket *socket;
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Command line parser with proper options
    QCommandLineParser parser;
    parser.setApplicationDescription("Client/Server Example");
    parser.addHelpOption();

    // Proper construction of options
    QCommandLineOption serverOption("server", "Run as server");
    QCommandLineOption clientOption("client", "Run as client");
    parser.addOption(serverOption);
    parser.addOption(clientOption);

    parser.process(app);

    bool isServer = parser.isSet("server");
    bool isClient = parser.isSet("client");

    if (isServer) {
        qDebug() << "Running in SERVER mode.";
        MyServer server;
        return app.exec();
    } else if (isClient) {
        qDebug() << "Running in CLIENT mode.";
        MyClient client;
        return app.exec();
    } else {
        qWarning() << "Please specify --server or --client.";
        return 1;
    }
}
