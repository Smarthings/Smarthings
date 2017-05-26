#ifndef CONNECTIONDATABASE_H
#define CONNECTIONDATABASE_H

#include <QObject>

class ConnectionDatabase : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionDatabase(QObject *parent = 0);

signals:

public slots:
};

#endif // CONNECTIONDATABASE_H