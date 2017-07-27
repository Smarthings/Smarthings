#include "tcpclient.h"

TcpClient::TcpClient(QStringList args, QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    QString server_addr;
    int server_port;

    if (args.count() > 1) {
        server_addr = QString(args.at(1));
        server_port = QString(args.at(2)).toInt();
    } else {
        server_addr = "localhost";
        server_port = 9876;
    }

    connect(socket, SIGNAL(connected()), this, SLOT(connectServer()));

    socket->connectToHost(server_addr, server_port, QIODevice::ReadWrite);
}

void TcpClient::connectServer()
{
    qDebug() << "Connect server";
    socket->flush();

    QByteArray data = socket->readAll();

    qDebug() << data;
}
