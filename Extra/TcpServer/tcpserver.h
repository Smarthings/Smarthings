#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QTimer>
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
    void sendDataClient();

protected:
    QTcpServer *server;
    QList<QTcpSocket *> clients;
};

#endif // TCPSERVER_H
