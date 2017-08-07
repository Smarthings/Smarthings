#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(incomingConnect()));

    if (!server->listen(QHostAddress::AnyIPv4, 9932)) {
        qDebug() << "Server error:" << server->errorString();
    }

    connect(this, SIGNAL(newConnect(QTcpSocket*)), this, SIGNAL(getAllNodes(QTcpSocket*)));
}

void TcpServer::incomingConnect()
{
    QTcpSocket *client = server->nextPendingConnection();
    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnectSocket()));

    clients.append(client);
    emit newConnect(client);
}

void TcpServer::readyRead()
{
    for (auto client: clients) {
        if (client->bytesAvailable()) {
            client->flush();
            QByteArray data = client->readAll();
            QJsonParseError parseError;
            QJsonDocument doc_tcp = QJsonDocument::fromJson(data, &parseError);

            if (parseError.error == 0) {
                for (const QJsonValue &json_object: doc_tcp.array()) {
                    for (const QString &key: json_object.toObject().keys()) {
                        if (key == "Node") {
                            emit sendCommandFromNode(json_object.toObject().value("Node").toObject());
                        }
                    }
                }
            }
        }
    }
}

void TcpServer::writeWithSocket(QJsonArray nodes, QTcpSocket *client)
{
    if (client->isWritable()) {
        QJsonDocument json_document(nodes);
        client->write(json_document.toJson());
    }
}

void TcpServer::writeAllSocket(QJsonArray nodes)
{
    QJsonDocument json_document(nodes);
    for (auto client: clients) {
        client->flush();
        client->write(json_document.toJson());
    }
}

void TcpServer::disconnectSocket()
{
    int i = 0;
    for (auto client: clients) {
        if (client->state() != QTcpSocket::ConnectedState) {
            //qDebug() << "Disconnect:" << client->peerAddress().toString() << client->peerPort();
            client->close();
            clients.removeAt(i);
            break;
        }
        i++;
    }
}
