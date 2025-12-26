#include <QCoreApplication>
#include <QProcess>
#include <QFile>
#include <QDebug>
#include <QCommandLineParser>
#include <QTextStream>
#include <QTemporaryFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Setup command line parser
    QCommandLineParser parser;
    parser.setApplicationDescription("FTP File Downloader");
    parser.addHelpOption();

    // Required positional arguments
    parser.addPositionalArgument("host", "FTP server host");
    parser.addPositionalArgument("remote_file", "Remote file path on FTP server");
    parser.addPositionalArgument("local_output_file", "Local filename to save as");

    // Optional username and password
    QCommandLineOption userOption(QStringList() << "u" << "user",
                                    "Username for FTP login",
                                    "username");
    QCommandLineOption passOption(QStringList() << "p" << "password",
                                    "Password for FTP login",
                                    "password");

    parser.addOption(userOption);
    parser.addOption(passOption);

    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if (args.size() != 3) {
        qWarning() << "ERROR: Incorrect number of positional arguments.";
        parser.showHelp(1);
        return 1;
    }

    QString host = args[0];
    QString remoteFile = args[1];
    QString localFileName = args[2];

    QString username = parser.value(userOption);
    QString password = parser.value(passOption);

    // Output standardized log messages for regression analysis
    qDebug() << "REGRESSION_START: FTP Download Test";
    qDebug() << "TARGET_HOST:" << host;
    qDebug() << "REMOTE_FILE:" << remoteFile;
    qDebug() << "LOCAL_FILE:" << localFileName;
    qDebug() << "USERNAME:" << username;

    // Create FTP command script using here-document
    QString ftpScript = QString(
        "#!/bin/ksh\n"
        "ftp -n %1 << 'FTPEOF'\n"
        "user %2 %3\n"
        "binary\n"
        "get %4 %5\n"
        "quit\n"
        "FTPEOF\n"
    ).arg(host).arg(username).arg(password).arg(remoteFile).arg(localFileName);

    // Write script to temporary file
    QTemporaryFile scriptFile;
    scriptFile.setAutoRemove(false);
    
    if (!scriptFile.open()) {
        qWarning() << "ERROR: Failed to create temporary FTP script file";
        return 1;
    }

    QTextStream out(&scriptFile);
    out << ftpScript;
    scriptFile.close();

    // Make script executable
    QFile::setPermissions(scriptFile.fileName(), 
                         QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);

    qDebug() << "FTP_SCRIPT_CREATED:" << scriptFile.fileName();
    qDebug() << "EXECUTING_FTP_DOWNLOAD";

    // Execute the script
    QProcess ftpProcess;
    ftpProcess.start("ksh", QStringList() << scriptFile.fileName());
    
    if (!ftpProcess.waitForStarted(5000)) {
        qWarning() << "ERROR: Failed to start FTP process";
        QFile::remove(scriptFile.fileName());
        return 1;
    }

    if (!ftpProcess.waitForFinished(30000)) { // 30 second timeout
        qWarning() << "ERROR: FTP process timed out";
        ftpProcess.kill();
        QFile::remove(scriptFile.fileName());
        return 1;
    }

    QString output = ftpProcess.readAllStandardOutput();
    QString error = ftpProcess.readAllStandardError();

    qDebug() << "FTP_PROCESS_EXIT_CODE:" << ftpProcess.exitCode();
    
    if (!output.isEmpty()) {
        qDebug() << "FTP_OUTPUT_START";
        qDebug() << output;
        qDebug() << "FTP_OUTPUT_END";
    }
    if (!error.isEmpty()) {
        qDebug() << "FTP_ERROR_START";
        qDebug() << error;
        qDebug() << "FTP_ERROR_END";
    }

    // Clean up script file
    QFile::remove(scriptFile.fileName());

    // Check if file was downloaded and report results
    if (QFile::exists(localFileName)) {
        QFile localFile(localFileName);
        qint64 fileSize = localFile.size();
        qDebug() << "DOWNLOAD_SUCCESS: File created";
        qDebug() << "FILE_SIZE:" << fileSize;
        qDebug() << "FILE_NAME:" << localFileName;
        
        if (fileSize > 0) {
            qDebug() << "REGRESSION_RESULT: DOWNLOAD_COMPLETED";
            qDebug() << "REGRESSION_END: SUCCESS";
            return 0;
        } else {
            qWarning() << "ERROR: Downloaded file is empty";
            qDebug() << "REGRESSION_RESULT: FILE_EMPTY";
            qDebug() << "REGRESSION_END: FAILURE";
            return 1;
        }
    } else {
        qWarning() << "ERROR: Download failed - file not created";
        qDebug() << "REGRESSION_RESULT: FILE_NOT_CREATED";
        qDebug() << "REGRESSION_END: FAILURE";
        return 1;
    }
}