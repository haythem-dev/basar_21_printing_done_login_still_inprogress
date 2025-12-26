#include <QCoreApplication>
#include <QTextCodec>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Original Unicode string with some non-ASCII characters
    QString originalString = "Hello, 世界!";

    // Get codecs for UTF-8 and Latin1
    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    QTextCodec *latin1Codec = QTextCodec::codecForName("ISO-8859-1");

    // Encode string to UTF-8 bytes
    QByteArray utf8Bytes = utf8Codec->fromUnicode(originalString);
    qDebug() << "UTF-8 bytes:" << utf8Bytes;

    // Encode string to Latin1 bytes (may lose characters)
    QByteArray latin1Bytes = latin1Codec->fromUnicode(originalString);
    qDebug() << "Latin1 bytes:" << latin1Bytes;

    // Decode back from UTF-8 bytes
    QString decodedUtf8 = utf8Codec->toUnicode(utf8Bytes);
    qDebug() << "Decoded from UTF-8:" << decodedUtf8;

    // Decode back from Latin1 bytes
    QString decodedLatin1 = latin1Codec->toUnicode(latin1Bytes);
    qDebug() << "Decoded from Latin1:" << decodedLatin1;

    return 0;
}
