#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if (!server->listen(QHostAddress::Any, 9876)) {
        qDebug() << "Server error: " << server->errorString();
    } else {
        qDebug() << "Server start";
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendDataClient()));
    timer->start(4000);

}

void TcpServer::newConnection()
{
    qDebug() << "new Connection";

    QTcpSocket *client = server->nextPendingConnection();
    connect(client, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnectClient()));

    clients.append(client);
}

void TcpServer::disconnectClient()
{
    int i = 0;
    for (auto client: clients) {
        if (client->state() != QTcpSocket::ConnectedState) {
            qDebug() << "Desconnect:" << client->peerAddress() << client->peerPort();
            client->close();
            clients.removeAt(i);
            break;
        }
        i++;
    }
}

void TcpServer::sendDataClient()
{
    for (auto client: clients) {
        qDebug() << "Send:" << client->peerAddress() << client->peerPort();
        client->flush();
        client->write("Data from server after 4 sec");
    }
}

void TcpServer::readSocket()
{
    for (auto client: clients) {
        if (client->bytesAvailable()) {
            qDebug() << "Read:" << client->peerAddress() << client->peerPort() << client->readAll();
        }
    }
}
