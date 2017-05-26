#ifndef SERIALSERVER_H
#define SERIALSERVER_H

#include <QObject>

class SerialServer : public QObject
{
    Q_OBJECT
public:
    explicit SerialServer(QObject *parent = 0);

signals:

public slots:
};

#endif // SERIALSERVER_H