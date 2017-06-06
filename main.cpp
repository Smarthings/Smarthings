#include <QCoreApplication>
#include <QThread>
#include <QObject>
#include <QMutex>

#include "cpp/udp/udpserver.h"
#include "cpp/tcp/tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UdpServer *udpServer = new UdpServer(0);
    TcpServer *tcpServer = new TcpServer(0);

    return a.exec();
}
