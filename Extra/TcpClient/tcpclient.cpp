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

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyFromServer()));
    socket->connectToHost(server_addr, server_port);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(writeSocket()));
    timer->start(5000);
}

void TcpClient::readyFromServer()
{
    qDebug() << "Connect server";
    socket->flush();

    QByteArray data;
    data = socket->readAll();

    qDebug() << data;
}

void TcpClient::writeSocket()
{
    qDebug() << "Write socket";
    socket->flush();
    QByteArray msg = "Message from client ";
    msg.append(QString(i++));
    socket->write(msg);
}
