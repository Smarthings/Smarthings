#include "socketclient.h"

SocketClient::SocketClient(QObject *parent) : QObject(parent)
{
    client = new QLocalSocket(this);
    client->connectToServer("/tmp/server", QIODevice::ReadWrite);

    connect(client, SIGNAL(readyRead()), this, SLOT(connectClient()));
}

void SocketClient::connectClient()
{
    qDebug() << client->readAll();
}
