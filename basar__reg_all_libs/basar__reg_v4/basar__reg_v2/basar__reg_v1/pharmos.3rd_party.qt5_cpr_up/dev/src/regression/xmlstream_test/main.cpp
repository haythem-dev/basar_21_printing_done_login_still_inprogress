#include <QCoreApplication>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QBuffer>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "Starting Qt XML Stream Test";
    qDebug() << "Based on -feature-xmlstream, -feature-xmlstreamreader, -feature-xmlstreamwriter flags";
    
    // Test 1: XML Stream Writer
    qDebug() << "\n=== Test 1: XML Stream Writer ===";
    
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    
    QXmlStreamWriter writer(&buffer);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    
    writer.writeStartElement("root");
    writer.writeAttribute("version", "1.0");
    
    writer.writeStartElement("person");
    writer.writeAttribute("id", "1");
    
    writer.writeTextElement("name", "John Doe");
    writer.writeTextElement("age", "30");
    writer.writeTextElement("city", "New York");
    
    writer.writeEndElement(); // person
    
    writer.writeStartElement("person");
    writer.writeAttribute("id", "2");
    
    writer.writeTextElement("name", "Jane Smith");
    writer.writeTextElement("age", "25");
    writer.writeTextElement("city", "Los Angeles");
    
    writer.writeEndElement(); // person
    
    writer.writeEndElement(); // root
    writer.writeEndDocument();
    
    buffer.close();
    
    QByteArray xmlData = buffer.data();
    qDebug() << "XML Writer test completed";
    qDebug() << "Generated XML size:" << xmlData.size() << "bytes";
    qDebug() << "XML content preview:" << QString::fromUtf8(xmlData.left(200));
    
    // Test 2: XML Stream Reader
    qDebug() << "\n=== Test 2: XML Stream Reader ===";
    
    QXmlStreamReader reader(xmlData);
    
    qDebug() << "XML Reader test started";
    
    while (!reader.atEnd()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        
        switch (token) {
        case QXmlStreamReader::StartDocument:
            qDebug() << "Start document";
            break;
            
        case QXmlStreamReader::EndDocument:
            qDebug() << "End document";
            break;
            
        case QXmlStreamReader::StartElement:
    qDebug() << "Start element:" << reader.name().toString();
    if (reader.attributes().size() > 0) { // Check if there are attributes
        QXmlStreamAttributes attributes = reader.attributes();
        for (const QXmlStreamAttribute &attr : attributes) {
            qDebug() << "  Attribute:" << attr.name().toString() << "=" << attr.value().toString();
        }
    }
    break;
            
        case QXmlStreamReader::EndElement:
            qDebug() << "End element:" << reader.name().toString();
            break;
            
        case QXmlStreamReader::Characters:
            if (!reader.isWhitespace()) {
                qDebug() << "Text:" << reader.text().toString();
            }
            break;
            
        case QXmlStreamReader::Comment:
            qDebug() << "Comment:" << reader.text().toString();
            break;
            
        case QXmlStreamReader::DTD:
            qDebug() << "DTD:" << reader.text().toString();
            break;
            
        case QXmlStreamReader::EntityReference:
            qDebug() << "Entity reference:" << reader.name().toString();
            break;
            
        case QXmlStreamReader::ProcessingInstruction:
            qDebug() << "Processing instruction:" << reader.processingInstructionTarget().toString();
            break;
            
        default:
            break;
        }
    }
    
    if (reader.hasError()) {
        qDebug() << "XML Reader error:" << reader.errorString();
        qDebug() << "Error at line:" << reader.lineNumber() << "column:" << reader.columnNumber();
    } else {
        qDebug() << "XML Reader test completed successfully";
    }
    
    // Test 3: Complex XML Structure
    qDebug() << "\n=== Test 3: Complex XML Structure ===";
    
    QBuffer complexBuffer;
    complexBuffer.open(QIODevice::WriteOnly);
    
    QXmlStreamWriter complexWriter(&complexBuffer);
    complexWriter.setAutoFormatting(true);
    complexWriter.writeStartDocument("1.0");
    
    complexWriter.writeStartElement("library");
    complexWriter.writeAttribute("name", "Qt Test Library");
    
    // Write books
    for (int i = 1; i <= 3; ++i) {
        complexWriter.writeStartElement("book");
        complexWriter.writeAttribute("id", QString::number(i));
        
        complexWriter.writeTextElement("title", QString("Book Title %1").arg(i));
        complexWriter.writeTextElement("author", QString("Author %1").arg(i));
        complexWriter.writeTextElement("year", QString::number(2020 + i));
        
        // Write chapters
        complexWriter.writeStartElement("chapters");
        for (int j = 1; j <= 3; ++j) {
            complexWriter.writeStartElement("chapter");
            complexWriter.writeAttribute("number", QString::number(j));
            complexWriter.writeTextElement("title", QString("Chapter %1").arg(j));
            complexWriter.writeTextElement("pages", QString::number(j * 10));
            complexWriter.writeEndElement(); // chapter
        }
        complexWriter.writeEndElement(); // chapters
        
        complexWriter.writeEndElement(); // book
    }
    
    complexWriter.writeEndElement(); // library
    complexWriter.writeEndDocument();
    
    complexBuffer.close();
    
    QByteArray complexXmlData = complexBuffer.data();
    qDebug() << "Complex XML generated, size:" << complexXmlData.size() << "bytes";
    
    // Parse complex XML
    QXmlStreamReader complexReader(complexXmlData);
    int bookCount = 0;
    int chapterCount = 0;
    
    while (!complexReader.atEnd()) {
        if (complexReader.readNext() == QXmlStreamReader::StartElement) {
            if (complexReader.name() == "book") {
                bookCount++;
            } else if (complexReader.name() == "chapter") {
                chapterCount++;
            }
        }
    }
    
    qDebug() << "Complex XML parsed successfully";
    qDebug() << "Books found:" << bookCount;
    qDebug() << "Chapters found:" << chapterCount;
    
    // Test 4: Namespace Support
    qDebug() << "\n=== Test 4: Namespace Support ===";
    
    QBuffer namespaceBuffer;
    namespaceBuffer.open(QIODevice::WriteOnly);
    
    QXmlStreamWriter namespaceWriter(&namespaceBuffer);
    namespaceWriter.setAutoFormatting(true);
    namespaceWriter.writeStartDocument();
    
    namespaceWriter.writeStartElement("root");
    namespaceWriter.writeDefaultNamespace("http://example.com/default");
    namespaceWriter.writeNamespace("http://example.com/custom", "custom");
    
    namespaceWriter.writeStartElement("http://example.com/default", "element1");
    namespaceWriter.writeCharacters("Default namespace content");
    namespaceWriter.writeEndElement();
    
    namespaceWriter.writeStartElement("http://example.com/custom", "element2");
    namespaceWriter.writeCharacters("Custom namespace content");
    namespaceWriter.writeEndElement();
    
    namespaceWriter.writeEndElement(); // root
    namespaceWriter.writeEndDocument();
    
    namespaceBuffer.close();
    
    QByteArray namespaceXmlData = namespaceBuffer.data();
    qDebug() << "Namespace XML generated, size:" << namespaceXmlData.size() << "bytes";
    
    // Parse namespace XML
    QXmlStreamReader namespaceReader(namespaceXmlData);
    
    while (!namespaceReader.atEnd()) {
        if (namespaceReader.readNext() == QXmlStreamReader::StartElement) {
            if (namespaceReader.name() == "element1" || namespaceReader.name() == "element2") {
                qDebug() << "Element:" << namespaceReader.name().toString();
                qDebug() << "  Namespace URI:" << namespaceReader.namespaceUri().toString();
                qDebug() << "  Prefix:" << namespaceReader.prefix().toString();
            }
        }
    }
    
    qDebug() << "Namespace test completed successfully";
    
    // Test 5: Error Handling
    qDebug() << "\n=== Test 5: Error Handling ===";
    
    QString invalidXml = "<root><unclosed><element>content</element></root>";
    QXmlStreamReader errorReader(invalidXml);
    
    while (!errorReader.atEnd()) {
        errorReader.readNext();
        if (errorReader.hasError()) {
            qDebug() << "Successfully detected XML error:" << errorReader.errorString();
            qDebug() << "Error at line:" << errorReader.lineNumber() << "column:" << errorReader.columnNumber();
            break;
        }
    }
    
    qDebug() << "\nQt XML Stream Test completed successfully";
    
    return 0;
}