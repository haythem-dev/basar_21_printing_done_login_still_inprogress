#include <QCoreApplication>
#include <QTranslator>
#include <QDebug>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Setup command-line parser
    QCommandLineParser parser;
    parser.setApplicationDescription("Qt I18n Example");
    parser.addHelpOption();

    // Add language option
    QCommandLineOption langOption("lang", "Set language locale (e.g., en, de, fr)", "locale");
    parser.addOption(langOption);
    parser.process(app);

    QString locale = parser.value("lang");
    if (locale.isEmpty()) {
        locale = "en"; // default to English if not specified
    }

    QTranslator translator;
    QString translationFile = "locale_" + locale + ".qm";

    if (translator.load(translationFile)) {
        QCoreApplication::installTranslator(&translator);
        qDebug() << "Loaded translation for language:" << locale;
    } else {
        qWarning() << "Translation file" << translationFile << "not found. Continuing with default language.";
    }

    // Your translatable string
    qDebug() << QObject::tr("Hello, world!");

    return 0;
}
