#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QJsonArray>

#include "tcpconnection.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();

signals:
    void getNodes(QJsonArray nodes);
    void getAllNodes();
    void sendCommandNode(QJsonObject object);

public slots:
    void nodesChanged(QJsonArray nodes);
    void receiveAllNodes(QJsonArray nodes);
    void CommandNode(QJsonObject object);

protected:
    int port;
    void incomingConnection(qintptr socketDescriptor);

private:
    QJsonArray *nodes_all;
};

#endif // TCPSERVER_H
