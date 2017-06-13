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

void TcpConnection::SocketWrite(QJsonArray nodes)
{
    if (socket->isWritable()) {
        /*QByteArray bytes;
        QDataStream out(&bytes, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_8);
        out << nodes.toVariantList();*/

        QJsonDocument json_doc(nodes);
        socket->write(json_doc.toJson());
    }
}
