#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QDomDocument>
#include <QStringList>
#include <QAbstractTableModel>
#include <vector>

// Custom table model
class HtmlTableModel : public QAbstractTableModel {
public:
    void setData(const std::vector<QStringList>& tableData) {
        data_ = tableData;
    }
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return static_cast<int>(data_.size());
    }
    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return data_.empty() ? 0 : static_cast<int>(data_[0].size());
    }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();
        return data_[index.row()][index.column()];
    }
private:
    std::vector<QStringList> data_;
};

// Parse the first HTML table into a vector of QStringLists
bool parseHtmlTable(const QString& html, std::vector<QStringList>& outTable) {
    QDomDocument doc;
    if (!doc.setContent(html))
        return false;

    QDomNodeList tables = doc.elementsByTagName("table");
    if (tables.isEmpty())
        return false;

    QDomElement table = tables.at(0).toElement();

    QDomNodeList rows = table.elementsByTagName("tr");
    for (int i = 0; i < rows.count(); ++i) {
        QDomElement row = rows.at(i).toElement();
        QStringList rowData;

        // support both <th> and <td>
        QDomNodeList cells = row.elementsByTagName("td");
        if (cells.isEmpty()) {
            cells = row.elementsByTagName("th");
        }

        for (int j = 0; j < cells.count(); ++j) {
            QDomElement cell = cells.at(j).toElement();
            QString text = cell.text().trimmed();
            rowData << text;
        }
        outTable.push_back(rowData);
    }
    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (argc != 2) {
        qWarning() << "Usage:" << argv[0] << "<input.html>";
        return 1;
    }

    QString inputFileName = argv[1];

    QFile file(inputFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open input file:" << inputFileName;
        return 1;
    }

    QString htmlContent = file.readAll();
    file.close();

    std::vector<QStringList> tableData;
    if (!parseHtmlTable(htmlContent, tableData) || tableData.empty()) {
        qWarning() << "Failed to parse HTML table.";
        return 1;
    }

    // Load data into model
    HtmlTableModel model;
    model.setData(tableData);

    // Calculate max width per column
    int colCount = model.columnCount();
    QVector<int> colWidths(colCount, 0);

    // Setup headers (dummy)
    QStringList headers;
    for (int col = 0; col < colCount; ++col) {
        QString headerText = QString("Col %1").arg(col + 1);
        headers << headerText;
        colWidths[col] = headerText.length();
    }

    // Determine max width for each column
    for (int row = 0; row < model.rowCount(); ++row) {
        for (int col = 0; col < colCount; ++col) {
            QModelIndex index = model.index(row, col);
            QString cellText = model.data(index).toString();
            if (cellText.length() > colWidths[col]) {
                colWidths[col] = cellText.length();
            }
        }
    }

    // Print header
    QString headerLine;
    for (int col = 0; col < colCount; ++col) {
        headerLine += headers[col].leftJustified(colWidths[col]) + " | ";
    }
    qDebug().noquote() << headerLine;

    // Build the separator line with correct alignment and vertical bars
    QString separatorLine;
    for (int col = 0; col < colCount; ++col) {
        separatorLine += QString("-").repeated(colWidths[col]);
        // Append vertical bar with padding if not last column
        separatorLine += (col == colCount - 1) ? "" : "-+-";
    }
    qDebug().noquote() << separatorLine;

    // Print data rows
    for (int row = 0; row < model.rowCount(); ++row) {
        QStringList rowStrings;
        for (int col = 0; col < colCount; ++col) {
            QModelIndex index = model.index(row, col);
            QString cellText = model.data(index).toString();
            rowStrings << cellText.leftJustified(colWidths[col]);
        }
        qDebug().noquote() << rowStrings.join(" | ");
    }

    return 0;
}
