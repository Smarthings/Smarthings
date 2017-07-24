#include "socketserver.h"

SocketServer::SocketServer(QObject *parent) : QObject(parent)
{
    server = new QLocalServer(this);
    if (!server->listen("server_mac")) {
        qDebug() << server->errorString();
    }
    connect(server, SIGNAL(newConnection()), this, SLOT(connectClient()));
}

void SocketServer::connectClient()
{
    qDebug() << "Connect";
    QLocalSocket *conClient = server->nextPendingConnection();
    connect(conClient, SIGNAL(disconnected()), conClient, SLOT(deleteLater()));
}
