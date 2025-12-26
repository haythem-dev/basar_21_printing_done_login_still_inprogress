#include <QCoreApplication>
#include <QDomDocument>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Sample XML string
    QString xmlString =
        "<library>"
        "  <book id='1'><title>Old Title</title></book>"
        "  <book id='2'><title>Another Book</title></book>"
        "</library>";

    // Parse XML into QDomDocument
    QDomDocument doc;
    QString errorMsg;
    int errorLine, errorColumn;
    if (!doc.setContent(xmlString, &errorMsg, &errorLine, &errorColumn)) {
        qWarning() << "Failed to parse XML:" << errorMsg << "at line" << errorLine << ", column" << errorColumn;
        return 1;
    }

    // Access root element
    QDomElement root = doc.documentElement();
    qDebug() << "Root element:" << root.tagName();

    // Traverse and print book titles
    QDomNodeList books = root.elementsByTagName("book");
    for (int i = 0; i < books.count(); ++i) {
        QDomNode node = books.at(i);
        QDomElement bookElem = node.toElement();
        QString id = bookElem.attribute("id");
        QDomElement titleElem = bookElem.firstChildElement("title");
        QString title = titleElem.text();
        qDebug() << "Book ID:" << id << "Title:" << title;
    }

    // Add a new book element
    QDomElement newBook = doc.createElement("book");
    newBook.setAttribute("id", "3");
    QDomElement newTitle = doc.createElement("title");
    newTitle.appendChild(doc.createTextNode("New Book Title"));
    newBook.appendChild(newTitle);
    root.appendChild(newBook);

    // Output the modified XML
    QString newXml = doc.toString(4); // pretty print with indent
    qDebug() << "Modified XML:\n" << newXml;

    return 0;
}
