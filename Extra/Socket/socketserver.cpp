#include "socketserver.h"

SocketServer::SocketServer(QObject *parent) : QObject(parent)
{
    server = new QLocalServer(this);
    if (!server->listen("/tmp/server")) {
        qDebug() << server->errorString();
    } else {
        qDebug() << "Socket start" << server->serverName();
    }
    connect(server, SIGNAL(newConnection()), this, SLOT(connectClient()));
}

void SocketServer::connectClient()
{
    qDebug() << "Connect";
    QLocalSocket *conClient = server->nextPendingConnection();
    connect(conClient, SIGNAL(disconnected()), conClient, SLOT(deleteLater()));
}

SocketServer::~SocketServer()
{
    server->disconnect();
    server->destroyed();
}
