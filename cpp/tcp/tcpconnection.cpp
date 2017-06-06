#include "tcpconnection.h"

TcpConnection::TcpConnection(qintptr id, QObject *parent) :
    QThread(parent)
{
    socketDescriptor = id;
}

void TcpConnection::run()
{
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << socket->error();
        return;
    }

    qDebug() << "Connect Socket: " << socketDescriptor;

    connect(socket, SIGNAL(readyRead()), this, SLOT(SocketRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    exec();
}

void TcpConnection::SocketRead()
{
    QByteArray Data = socket->readAll();
    qDebug() << Data;
}

void TcpConnection::disconnected()
{
    qDebug() << "Desconnect: " << socketDescriptor;
    socket->deleteLater();
    exit(0);
}
