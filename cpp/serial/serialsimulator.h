#ifndef SERIALSIMULATOR_H
#define SERIALSIMULATOR_H

#include <QObject>
#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

class SerialSimulator : public QObject
{
    Q_OBJECT
public:
    explicit SerialSimulator(QObject *parent = nullptr);
    void CreateNodes();

signals:
    void getSerial(QString read_data);

public slots:

protected:

};

#endif // SERIALSIMULATOR_H
