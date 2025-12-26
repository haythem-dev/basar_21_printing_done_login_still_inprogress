#include <QCoreApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if (argc != 3) {
        qWarning() << "Usage:" << argv[0] << "<input.html> <output.odf>";
        return 1;
    }

    QString inputPath = argv[1];
    QString outputPath = argv[2];

    // Read HTML file
    QFile inputFile(inputPath);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open input:" << inputPath;
        return 1;
    }
    QByteArray htmlData = inputFile.readAll();
    inputFile.close();

    // **Simple plain text extraction:** (e.g., remove HTML tags with regex)
    QString htmlText = QString::fromUtf8(htmlData);
    QRegExp tagExp("<[^>]*>");
    QString plainText = htmlText;
    plainText.replace(tagExp, "");

    // Write some ODF content manually (very simplified)
    QFile outputFile(outputPath);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open output:" << outputPath;
        return 1;
    }
    QByteArray odfContent = R"(
<?xml version="1.0" encoding="UTF-8"?>
<office:document-content xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
                        xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0">
  <office:body>
    <office:text>
      <text:p>)" + plainText.toUtf8() + "</text:p>" +
                  R"(</office:text>
  </office:body>
</office:document-content>)";

    outputFile.write(odfContent);
    outputFile.close();

    qDebug() << "Plain text extracted and ODF created.";
    return 0;
}
