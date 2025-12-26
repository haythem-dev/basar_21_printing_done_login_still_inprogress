#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <QStringList>

int main(int argc, char *argv[])
{
  if (argc < 3) {
    qWarning() << "Usage:" << argv[0] << "<pattern> <file1> [<file2> ...]";
    return 1;
  }

  QString pattern = argv[1];
  QStringList files;
  for (int i = 2; i < argc; ++i) {
    files << argv[i];
  }

  QRegularExpression regex(pattern);
  // To use case-insensitive matching, uncomment:
  // regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

  for (const QString &filename : files) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qWarning() << "Cannot open file:" << filename;
      continue; // skip to next file
    }

    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
      lines << in.readLine();
    }

    for (int i = 0; i < lines.size(); ++i) {
      const QString &line = lines[i];
      int lineNumber = i + 1;

      QRegularExpressionMatchIterator it = regex.globalMatch(line);
      while (it.hasNext()) {
	QRegularExpressionMatch match = it.next();
	QString matchedText = match.captured(0);
	int startIndex = match.capturedStart(0);
	int columnNumber = startIndex + 1; // columns are 1-based

	// Output match info in the format: filename:line:column:match
	qDebug().noquote() << filename + ":" + QString::number(lineNumber) + ":" + QString::number(columnNumber) + ":" + matchedText;

	// Output the entire line below
	qDebug().noquote() << line;
      }
    }
  }

  return 0;
}
