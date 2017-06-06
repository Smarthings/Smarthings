#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>

class UdpServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpServer(QObject *parent = 0);

signals:
    void hostConnect(QHostAddress ip_addresss);

public slots:
    void bind();
    void getClient();
    void sendMessage(QHostAddress ip_address);

private:
    QUdpSocket *udpServer;
};

#endif // UDPSERVER_H
