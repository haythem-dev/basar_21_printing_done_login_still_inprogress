#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>
#include <QCommandLineParser>

class HelloObject : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE QString hello() const {
        return "Hello, world!";
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOptions({{"server", "Run as D-Bus server"}, {"client", "Run as D-Bus client"}});
    parser.process(app);

    bool isServer = parser.isSet("server");
    bool isClient = parser.isSet("client");

    if (isServer && isClient) {
        qWarning() << "Cannot be both server and client.";
        return 1;
    }

    const QString serviceName = "com.example.HelloService";
    const QString objectPath = "/com/example/HelloObject";
    const QString interfaceName = "com.example.HelloObject";

    if (isServer) {
        // Run as server
        HelloObject obj;
        QDBusConnection sessionBus = QDBusConnection::sessionBus();

        if (!sessionBus.registerService(serviceName)) {
            qWarning() << "Failed to register service:" << sessionBus.lastError().message();
            return 1;
        }

        if (!sessionBus.registerObject(objectPath, &obj, QDBusConnection::ExportAllSlots)) {
            qWarning() << "Failed to register object:" << sessionBus.lastError().message();
            return 1;
        }

        qDebug() << "D-Bus server is running. Press Ctrl+C to exit.";
        return app.exec();

    } else if (isClient) {
        // Run as client
        QDBusInterface iface(serviceName, objectPath, interfaceName, QDBusConnection::sessionBus());
        if (!iface.isValid()) {
            qWarning() << "Failed to connect to service:" << iface.lastError().message();
            return 1;
        }
        QDBusReply<QString> reply = iface.call("hello");
        if (reply.isValid()) {
            qDebug() << "Response from server:" << reply.value();
        } else {
            qWarning() << "Call failed:" << reply.error().message();
            return 1;
        }
        return 0;

    } else {
        qWarning() << "Please specify --server or --client";
        return 1;
    }
}

#include "main.moc"
