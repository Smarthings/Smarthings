#include <QCoreApplication>
#include <QDebug>
#include "./tcpclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpClient tcpClient(a.arguments());

    return a.exec();
}
