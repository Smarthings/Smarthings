#include <QCoreApplication>
#include <QThread>
#include <QObject>

#include <QJsonArray>

#include "cpp/udp/udpserver.h"
#include "cpp/tcp/tcpserver.h"
#include "cpp/serial/serialserver.h"
#include "cpp/serial/nodes.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Nodes slaves;
    SerialServer serialServer;
    QObject::connect(&serialServer, SIGNAL(getSerial(QString)), &slaves, SLOT(addNodes(QString)));

    UdpServer udpServer;
    TcpServer tcpServer;

    QObject::connect(&tcpServer, SIGNAL(getAllNodes(QTcpSocket*)), &slaves, SLOT(requireGetAllNodes(QTcpSocket*)));
    QObject::connect(&slaves, SIGNAL(sendAllNodes(QJsonArray, QTcpSocket*)), &tcpServer, SLOT(writeWithSocket(QJsonArray, QTcpSocket*)));
    QObject::connect(&slaves, SIGNAL(updateNodes(QJsonArray)), &tcpServer, SLOT(writeAllSocket(QJsonArray)));
    QObject::connect(&tcpServer, SIGNAL(sendCommandFromNode(QJsonObject)), &serialServer, SLOT(receiveCommand(QJsonObject)));

    return a.exec();
}
