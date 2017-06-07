#include <QCoreApplication>
#include <QThread>
#include <QObject>
#include <QMutex>

#include "cpp/udp/udpserver.h"
#include "cpp/tcp/tcpserver.h"
#include "cpp/serial/serialserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SerialServer serialServer;
    UdpServer udpServer;
    TcpServer tcpServer;

    return a.exec();
}
