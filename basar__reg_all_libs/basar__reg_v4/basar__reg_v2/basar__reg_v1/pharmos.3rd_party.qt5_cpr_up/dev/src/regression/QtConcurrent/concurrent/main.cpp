#include <QCoreApplication>
#include <QtConcurrent>
#include <QFuture>
#include <QList>
#include <QDebug>

// Function to calculate the square of a number
int square(int n) {
    return n * n;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Create a list of integers
    QList<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Use QtConcurrent::mapped to process in parallel
    QFuture<int> future = QtConcurrent::mapped(numbers, square);

    // Wait for the computation to finish
    future.waitForFinished();

    // Retrieve the results
    QList<int> squares = future.results();

    // Output the results
    qDebug() << "Numbers:" << numbers;
    qDebug() << "Squares:" << squares;

    return 0;
}
