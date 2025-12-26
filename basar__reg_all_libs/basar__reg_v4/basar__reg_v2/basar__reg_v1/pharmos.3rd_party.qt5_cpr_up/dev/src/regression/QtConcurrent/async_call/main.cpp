#include <QCoreApplication>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QDebug>

// An example function that "calls" a stored function (simulate work)
int callStoredFunction() {
    // Simulate some work, e.g., network call or computation
    QThread::sleep(2); // sleep 2 seconds
    return 42; // Example return value
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QFuture<int> future = QtConcurrent::run(callStoredFunction);

    QFutureWatcher<int> watcher;
    QObject::connect(&watcher, &QFutureWatcher<int>::finished, [&]() {
        int result = watcher.result();
        qDebug() << "Result from stored function:" << result;
        QCoreApplication::quit();
    });
    watcher.setFuture(future);

    return app.exec();
}
