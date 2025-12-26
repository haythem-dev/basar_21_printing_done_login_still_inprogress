#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "Starting JSON Processing Test";
    
    // Create JSON object
    QJsonObject person;
    person["name"] = "John Doe";
    person["age"] = 30;
    person["active"] = true;
    
    // Create JSON array
    QJsonArray hobbies;
    hobbies.append("reading");
    hobbies.append("programming");
    hobbies.append("gaming");
    
    person["hobbies"] = hobbies;
    
    // Create nested object
    QJsonObject address;
    address["street"] = "123 Main St";
    address["city"] = "Anytown";
    address["zipcode"] = "12345";
    
    person["address"] = address;
    
    qDebug() << "JSON object created";
    
    // Convert to document and JSON string
    QJsonDocument doc(person);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    
    qDebug() << "JSON string (compact):" << QString::fromUtf8(jsonData);
    
    // Pretty print
    QByteArray prettyJson = doc.toJson(QJsonDocument::Indented);
    qDebug() << "JSON string (pretty):\n" << QString::fromUtf8(prettyJson);
    
    // Parse JSON string
    QJsonParseError error;
    QJsonDocument parsedDoc = QJsonDocument::fromJson(jsonData, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qCritical() << "JSON parsing failed:" << error.errorString();
        return 1;
    }
    
    qDebug() << "JSON parsed successfully";
    
    // Access parsed data
    QJsonObject parsedPerson = parsedDoc.object();
    
    qDebug() << "Parsed data:";
    qDebug() << "Name:" << parsedPerson["name"].toString();
    qDebug() << "Age:" << parsedPerson["age"].toInt();
    qDebug() << "Active:" << parsedPerson["active"].toBool();
    
    // Access array
    QJsonArray parsedHobbies = parsedPerson["hobbies"].toArray();
    qDebug() << "Hobbies:";
    for (int i = 0; i < parsedHobbies.size(); ++i) {
        qDebug() << "  -" << parsedHobbies[i].toString();
    }
    
    // Access nested object
    QJsonObject parsedAddress = parsedPerson["address"].toObject();
    qDebug() << "Address:";
    qDebug() << "  Street:" << parsedAddress["street"].toString();
    qDebug() << "  City:" << parsedAddress["city"].toString();
    qDebug() << "  Zipcode:" << parsedAddress["zipcode"].toString();
    
    // Test JSON array operations
    QJsonArray numbers;
    for (int i = 1; i <= 5; ++i) {
        numbers.append(i * 10);
    }
    
    qDebug() << "Number array:" << QJsonDocument(numbers).toJson(QJsonDocument::Compact);
    
    // Test JSON value types
    QJsonObject types;
    types["string"] = "Hello";
    types["integer"] = 42;
    types["double"] = 3.14159;
    types["boolean"] = true;
    types["null"] = QJsonValue();
    
    qDebug() << "Data types:" << QJsonDocument(types).toJson(QJsonDocument::Compact);
    
    // Test malformed JSON
    QJsonDocument invalidDoc = QJsonDocument::fromJson("{invalid json}", &error);
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Successfully detected invalid JSON:" << error.errorString();
    }
    
    qDebug() << "JSON Processing Test completed successfully";
    
    return 0;
}