#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>

#include "tcpconnection.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();

signals:

public slots:

protected:
    int port;
    void incomingConnection(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
