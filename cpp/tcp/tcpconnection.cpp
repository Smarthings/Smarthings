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
    socket->flush();
    QByteArray data = socket->readAll();
    QJsonParseError parseError;
    QJsonDocument doc_tcp = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error == 0) {
        qDebug() << doc_tcp.array();
    }
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
        QJsonDocument json_doc(nodes);
        socket->write(json_doc.toJson());
    }
}
