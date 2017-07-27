#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

signals:

public slots:
    void newConnection();

protected:
    QTcpServer *server;
    QList<QTcpSocket *> clients;
};

#endif // TCPSERVER_H
