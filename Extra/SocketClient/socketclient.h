#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QLocalSocket>
#include <QDebug>

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = nullptr);

signals:

public slots:
    void connectClient();

private:
    QLocalSocket *client;
};

#endif // SOCKETCLIENT_H
