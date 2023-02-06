#include <QCommandLineParser>
#include <QCoreApplication>
#include "logger.h"
#include "signalhelper.h"
#include "commandlineparserhelper.h"
#include "work1.h"

auto main(int argc, char *argv[]) -> int
{
    Logger::Init(Logger::ErrLevel::INFO, Logger::DbgLevel::TRACE, true, true);

    SignalHelper::setShutDownSignal(SignalHelper::SIGINT_); // shut down on ctrl-c
    SignalHelper::setShutDownSignal(SignalHelper::SIGTERM_); // shut down on killall

    //zInfo(QStringLiteral("started"));

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("test1"));

    QCommandLineParser parser;

    parser.setApplicationDescription(QStringLiteral("command line test1 app."));
    parser.addHelpOption();
    parser.addVersionOption();  

    //const QString OPTION_IP = QStringLiteral("ip");
    const QString OPTION_INFILE = QStringLiteral("input");
    const QString OPTION_OUTFILE = QStringLiteral("output");
//    const QString OPTION_BACKUP = QStringLiteral("backup");

//    CommandLineParserHelper::addOption(&parser, OPTION_IP, QStringLiteral("ipaddress"));
    CommandLineParserHelper::addOption(&parser, OPTION_INFILE, QStringLiteral("input file"));
    CommandLineParserHelper::addOptionBool(&parser, OPTION_OUTFILE, QStringLiteral("output file"));

    parser.process(app);

    Work1 w1;

    QString inFile = parser.value(OPTION_INFILE);
    QString outFile = parser.value(OPTION_OUTFILE);

    Work1::Params w1Params(inFile, outFile);
    auto w1Result = w1.doWork(w1Params); // indítás direkt   

    return (w1Result.state==Work1::Result::Ok)?0:-1;
}


