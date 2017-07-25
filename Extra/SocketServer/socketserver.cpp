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
    client = server->nextPendingConnection();
    connect(client, SIGNAL(disconnected()), this, SLOT(closeClient()));

    QByteArray block;
    block = "Teste Connect";

    client->write(block);
    client->flush();
}

void SocketServer::closeClient()
{
    qDebug() << "Close client";
    client->close();
}

SocketServer::~SocketServer()
{
    server->disconnect();
    server->destroyed();
}
