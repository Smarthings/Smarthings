#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTcpSocket>

class TcpConnection : public QThread
{
    Q_OBJECT
public:
    explicit TcpConnection(qintptr id, QObject *parent = 0);
    void run();

signals:

public slots:
    void SocketRead();
    void disconnected();

private:
    qintptr socketDescriptor;
    QTcpSocket *socket;
};

#endif // TCPCONNECTION_H
