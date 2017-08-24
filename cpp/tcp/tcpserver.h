#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QList>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

signals:
    void newConnect(QTcpSocket *client);
    void getAllNodes(QTcpSocket *client);
    void sendCommandFromNode(QJsonObject object);

public slots:
    void incomingConnect();
    void readyRead();
    void disconnectSocket();
    void writeWithSocket(QJsonArray nodes, QTcpSocket *client);
    void writeAllSocket(QJsonArray nodes);

private:
    QTcpServer *server;
    QList<QTcpSocket *> clients;
};

#endif // TCPSERVER_H
