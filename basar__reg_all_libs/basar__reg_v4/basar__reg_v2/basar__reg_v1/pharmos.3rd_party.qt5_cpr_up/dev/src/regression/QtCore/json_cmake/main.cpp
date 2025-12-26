#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // 1. Create a JSON object
    QJsonObject jsonObj;
    jsonObj["name"] = "Alice";
    jsonObj["age"] = 30;
    jsonObj["is_student"] = false;

    // 2. Convert QJsonObject to JSON string
    QJsonDocument jsonDoc(jsonObj);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact); // or Qt::Indented

    qDebug() << "Serialized JSON:\n" << jsonString;

    // 3. Parse JSON string back to object
    QJsonDocument parseDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (parseDoc.isNull()) {
        qWarning() << "Failed to parse JSON!";
        return 1;
    }

    QJsonObject parsedObj = parseDoc.object();

    qDebug() << "Parsed JSON object:";
    qDebug() << "Name:" << parsedObj["name"].toString();
    qDebug() << "Age:" << parsedObj["age"].toInt();
    qDebug() << "Is student:" << parsedObj["is_student"].toBool();

    return 0;
}
