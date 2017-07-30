#ifndef SERIALSERVER_H
#define SERIALSERVER_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QJsonObject>
#include <QJsonValue>

class SerialServer : public QObject
{
    Q_OBJECT
public:
    explicit SerialServer(QObject *parent = 0);
    void ConfigureSerial();
    void openSerialPort();
    QStringList AvailablePorts();

signals:
    void readyCmd();
    void cmdWait(const QByteArray &data);
    void getSerial(const QString read_serial);

public slots:
    void writeSerial(const QJsonObject nodes);
    void receiveCommand(const QJsonObject commands);
    void writeData(const QByteArray &data);
    void readData();

protected:
    void prepareSerial(const QJsonObject node);

private:
    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError);
    QSerialPort *serial;
};

#endif // SERIALSERVER_H
