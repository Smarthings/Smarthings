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

    clients.push_back(server->nextPendingConnection());

    qDebug() << "Clients: " << clients.count();

    clients.at(0)->write("Qualquer coisa");

}

void TcpServer::sendDataClient()
{
    for (auto client: clients) {
        qDebug() << "Send data to client" << client->peerAddress() << client->peerPort();
        client->flush();
        client->write("Data from server after 4 sec");
    }
}
