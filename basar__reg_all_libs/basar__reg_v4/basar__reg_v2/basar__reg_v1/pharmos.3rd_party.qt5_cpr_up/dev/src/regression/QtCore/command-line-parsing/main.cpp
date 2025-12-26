#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream out(stdout);

    // Set up command line parser
    QCommandLineParser parser;
    parser.setApplicationDescription("Command Line Parsing Example with QCommandLineParser");
    parser.addHelpOption();

    // Add --name option
    QCommandLineOption nameOption(QStringList() << "n" << "name",
                                      "Specify a name to greet.",
                                      "name");
    parser.addOption(nameOption);

    // Process the actual command line arguments
    parser.process(app);

    if (parser.isSet("help")) {
        parser.showHelp();
        return 0; // showHelp() will terminate the app
    }

    // Get the value of --name, if provided
    QString name = parser.value("name");

    if (!name.isEmpty()) {
        out << "Hello, " << name << "!\n";
    } else {
        out << "Hello, World!\n";
    }

    return 0;
}
