#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDebug>

class SocketServer : public QObject
{
    Q_OBJECT
public:
    explicit SocketServer(QObject *parent = nullptr);
    ~SocketServer();

signals:

public slots:
    void connectClient();

private:
    QLocalServer *server;
};

#endif // SOCKETSERVER_H
