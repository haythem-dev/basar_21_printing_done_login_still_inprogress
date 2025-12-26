#include <QCoreApplication>
#include <QDate>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream cin(stdin);
    QTextStream cout(stdout);

    // Expecting two dates as arguments in format YYYY-MM-DD
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <date1> <date2>\n";
        cout << "Dates in format YYYY-MM-DD\n";
        return 1;
    }

    QString dateStr1 = argv[1];
    QString dateStr2 = argv[2];

    QDate date1 = QDate::fromString(dateStr1, "yyyy-MM-dd");
    QDate date2 = QDate::fromString(dateStr2, "yyyy-MM-dd");

    if (!date1.isValid() || !date2.isValid()) {
        cout << "Invalid date format. Please use YYYY-MM-DD.\n";
        return 1;
    }

    // Output weekdays
    cout << "Date 1: " << date1.toString("yyyy-MM-dd") << " is a " << date1.toString("dddd") << "\n";
    cout << "Date 2: " << date2.toString("yyyy-MM-dd") << " is a " << date2.toString("dddd") << "\n";

    // Calculate difference in days
    int daysDiff = qAbs(date1.daysTo(date2));
    cout << "Difference in days: " << daysDiff << "\n";

    // Determine earlier date and print month calendar
    QDate startDate = (date1 < date2) ? date1 : date2;

    int year = startDate.year();
    int month = startDate.month();

    // Print month calendar
    QDate firstDayOfMonth(year, month, 1);
    int daysInMonth = firstDayOfMonth.daysInMonth();

    // Print header
    QString monthName = firstDayOfMonth.toString("MMMM yyyy");
    cout << "\n     " << monthName << "\n";
    cout << "Su Mo Tu We Th Fr Sa\n";

    // Find the weekday of the first day (0=Sunday, 6=Saturday)
    int startDay = firstDayOfMonth.dayOfWeek() % 7; // QDate::dayOfWeek(): Monday=1, Sunday=7

    // Print initial spaces
    for (int i = 0; i < startDay; ++i) {
        cout << "   ";
    }

    // Print all days
    for (int day = 1; day <= daysInMonth; ++day) {
        cout << QString("%1").arg(day, 2, 10, QLatin1Char(' ')) << " ";
        if ((startDay + day) % 7 == 0) {
            cout << "\n"; // New line at week end
        }
    }
    cout << "\n";

    return 0;
}
