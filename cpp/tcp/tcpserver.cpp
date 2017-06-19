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
    connect(newConn, SIGNAL(started()), this, SIGNAL(getAllNodes()));
    connect(this, SIGNAL(getNodes(QJsonArray)), newConn, SLOT(SocketWrite(QJsonArray)));
    connect(newConn, SIGNAL(getCommandNode(QJsonObject)), this, SLOT(CommandNode(QJsonObject)));
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
 void TcpServer::receiveAllNodes(QJsonArray nodes)
 {
    qDebug() << "receive all nodes";
    emit getNodes(nodes);
 }

 void TcpServer::CommandNode(QJsonObject object)
 {
     emit sendCommandNode(object);
 }
