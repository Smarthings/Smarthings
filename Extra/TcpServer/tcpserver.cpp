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
}

void TcpServer::newConnection()
{
    qDebug() << "new Connection";

    clients.push_back(server->nextPendingConnection());

    qDebug() << "Clients: " << clients.count();

    clients.at(0)->write("Qualquer coisa");

}
