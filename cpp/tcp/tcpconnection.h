#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDataStream>

class TcpConnection : public QThread
{
    Q_OBJECT
public:
    explicit TcpConnection(qintptr id, QObject *parent = 0);
    void run();

signals:

public slots:
    void disconnected();
    void SocketWrite(QJsonArray nodes);
    void SocketRead();

private:
    qintptr socketDescriptor;
    QTcpSocket *socket;

    QJsonArray *data;
};

#endif // TCPCONNECTION_H
