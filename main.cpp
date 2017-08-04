#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QThread>
#include <QObject>

#include <QJsonArray>

#include "cpp/udp/udpserver.h"
#include "cpp/tcp/tcpserver.h"
#include "cpp/serial/serialserver.h"
#include "cpp/serial/nodes.h"

#include "cpp/serial/serialsimulator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("smarthings");
    QCoreApplication::setApplicationVersion("0.1a");

    QCommandLineParser parser;
    parser.setApplicationDescription("Help Smarthings");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption showSimulatorSerial({"s", "simulate"}, QCoreApplication::translate("simulate", "Simulate serial - example of nodes"));
    parser.addOption(showSimulatorSerial);
    parser.process(a);

    bool simulate = parser.isSet(showSimulatorSerial);

    Nodes slaves;
    SerialServer serialServer(simulate);
    QObject::connect(&serialServer, SIGNAL(getSerial(QString)), &slaves, SLOT(addNodes(QString)));

    SerialSimulator serialSimulator;
    QObject::connect(&serialSimulator, SIGNAL(getSerial(QString)), &slaves, SLOT(addNodes(QString)));

    if (simulate) {
        qDebug() << "[ Ok ] Simulator mode";
        serialSimulator.CreateNodes();
        QObject::connect(&serialServer, SIGNAL(writeSerialSimulate(QJsonObject)), &slaves, SLOT(writeSerialSimulate(QJsonObject)));
    }

    UdpServer udpServer;
    TcpServer tcpServer;

    QObject::connect(&tcpServer, SIGNAL(getAllNodes(QTcpSocket*)), &slaves, SLOT(requireGetAllNodes(QTcpSocket*)));
    QObject::connect(&slaves, SIGNAL(sendAllNodes(QJsonArray, QTcpSocket*)), &tcpServer, SLOT(writeWithSocket(QJsonArray, QTcpSocket*)));
    QObject::connect(&serialServer, SIGNAL(sendStopwatchCommand(QJsonArray)), &tcpServer, SLOT(writeAllSocket(QJsonArray)));
    QObject::connect(&slaves, SIGNAL(updateNodes(QJsonArray)), &tcpServer, SLOT(writeAllSocket(QJsonArray)));
    QObject::connect(&tcpServer, SIGNAL(sendCommandFromNode(QJsonObject)), &serialServer, SLOT(receiveCommand(QJsonObject)));

    return a.exec();
}
