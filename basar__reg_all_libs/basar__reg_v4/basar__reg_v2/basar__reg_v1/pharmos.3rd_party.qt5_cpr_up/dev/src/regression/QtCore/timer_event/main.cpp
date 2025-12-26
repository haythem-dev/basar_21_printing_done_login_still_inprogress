#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

// Slot function to handle timer timeout
void onTimeout() {
    qDebug() << "Timeout occurred! Event handled.";
    QCoreApplication::quit(); // Exit the event loop
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    qDebug() << "Application started. Waiting for event...";

    // Set a timer for 3 seconds
    QTimer::singleShot(3000, &onTimeout);

    // Run the event loop
    return app.exec();
}
