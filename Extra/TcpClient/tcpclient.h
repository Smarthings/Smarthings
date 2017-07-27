#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QtCore>
#include <QDebug>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QStringList args, QObject *parent = nullptr);

signals:

public slots:
    void readyFromServer();

private:
    QTcpSocket *socket;
};

#endif // TCPCLIENT_H
