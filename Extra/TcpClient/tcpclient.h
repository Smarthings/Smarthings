#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QtCore>
#include <QDebug>
#include <QTimer>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QStringList args, QObject *parent = nullptr);

signals:

public slots:
    void readyFromServer();
    void writeSocket();

private:
    QTcpSocket *socket;
    int i = 0;
};

#endif // TCPCLIENT_H
