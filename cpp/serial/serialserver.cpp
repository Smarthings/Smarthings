#include "serialserver.h"

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SerialServer::SerialServer(QObject *parent) :
    QObject(parent)
{
    serial = new QSerialPort(this);
    this->ConfigureSerial();
    this->openSerialPort();

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &SerialServer::handleError);
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(this, SIGNAL(cmdWait(QByteArray)), this, SLOT(writeData(QByteArray)));
}

void SerialServer::ConfigureSerial()
{
    qDebug() << "Configure Serial";
    QStringList ports = this->AvailablePorts();
    serial->setPortName(ports.at(4));
    serial->setBaudRate(QSerialPort::Baud57600, QSerialPort::AllDirections);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
}

void SerialServer::openSerialPort()
{
    qDebug() << "Open Serial Port";
    if (serial->open(QIODevice::ReadWrite)) {
        qDebug() << "[ Ok ] Connect Serial";
    } else {
        qDebug() << serial->errorString();
    }
}

QStringList SerialServer::AvailablePorts()
{
    QString description,
            manufacturer,
            serialNumber;
    QStringList listPorts;

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info: infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);
        listPorts.append(list);
    }
    return listPorts;
}

void SerialServer::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
}

void SerialServer::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qDebug() << serial->errorString();
        closeSerialPort();
    }
}

void SerialServer::readData()
{
    QString read_data;
    while (serial->waitForReadyRead(75)) {
        read_data.append(serial->readAll());
    }
    read_data.replace("\n", "");
    emit getSerial(read_data);
}

void SerialServer::writeData(const QByteArray &data)
{
    serial->write(data);
}

void SerialServer::writeSerial(const QJsonObject nodes)
{
    for (const QJsonValue &node: nodes) {
        QByteArray node_write;
        node_write.append(node.toString());
        serial->write(node_write);
    }
}

void SerialServer::prepareSerial(const QJsonObject nodes)
{
    QStringList nodes_list = nodes.keys();
    for (const QString &node: nodes_list) {
        QString range = nodes[node].toObject().value("action").toObject().value("range").toString();
        QStringList str_node;
        str_node << "#0" << range << "00" << node << ":";

        serial->write(QString(str_node.join("")).toUtf8());
    }
}
