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
    QObject::connect(&tcpServer, SIGNAL(getAllNodes()), &slaves, SLOT(requireGetAllNodes()));
    QObject::connect(&slaves, SIGNAL(sendAllNodes(QJsonArray)), &tcpServer, SLOT(receiveAllNodes(QJsonArray)));
    QObject::connect(&slaves, SIGNAL(updateNodes(QJsonArray)), &tcpServer, SLOT(nodesChanged(QJsonArray)));
    QObject::connect(&tcpServer, SIGNAL(sendCommandNode(QJsonObject)), &serialServer, SLOT(prepareSerial(QJsonObject)));

    return a.exec();
}
