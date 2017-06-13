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

    QJsonArray nodes;

    Nodes slaves(&nodes);
    SerialServer serialServer;
    QObject::connect(&serialServer, SIGNAL(getSerial(QString)), &slaves, SLOT(addNodes(QString)));

    UdpServer udpServer;
    TcpServer tcpServer;
    QObject::connect(&slaves, SIGNAL(updateNodes(QJsonArray)), &tcpServer, SLOT(nodesChanged(QJsonArray)));

    return a.exec();
}
