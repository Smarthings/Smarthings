#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{
    port = 9932;
    if (!this->listen(QHostAddress::AnyIPv4, port))
        qDebug() << "Could not start server";

}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    TcpConnection *newConn = new TcpConnection(socketDescriptor, this);
    connect(newConn, SIGNAL(finished()), newConn, SLOT(deleteLater()));
    connect(this, SIGNAL(getNodes(QJsonArray)), newConn, SLOT(SocketWrite(QJsonArray)));
    newConn->start();
}

TcpServer::~TcpServer()
{
    this->disconnect();
}

void TcpServer::nodesChanged(QJsonArray nodes)
{
    emit getNodes(nodes);
}
