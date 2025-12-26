#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString inputString = "Hello, SHA3!";
    QByteArray inputBytes = inputString.toUtf8();

    // Compute SHA3-256 hash
    QByteArray hashBytes = QCryptographicHash::hash(inputBytes, QCryptographicHash::Sha3_256);

    // Convert to hex string
    QString hashString = hashBytes.toHex();

    qDebug() << "Input String:" << inputString;
    qDebug() << "SHA3-256 Hash:" << hashString;

    return 0;
}
