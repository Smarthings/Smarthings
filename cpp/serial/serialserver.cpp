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

void SerialServer::receiveCommand(const QJsonObject commands)
{
    QStringList command_list = commands.keys();
    for (const QString &it: command_list) {
        QStringList command = commands[it].toObject().keys();
        for (const QString &key: command) {
            if (key == "action") {
                QJsonObject node;
                node.insert(it, commands[it].toObject().value(key));

                prepareSerial(node);
                continue;
            }
            if (key == "stopwatch") {
                QJsonObject::const_iterator i = commands[it].toObject().value(key).toObject().find("time");
                if (commands[it].toObject().value(key).toObject().contains("time") && i.key() == "time") {
                    if (commands[it].toObject().value(key).toObject().contains("action")) {
                        QJsonObject timestamp, objts, node;

                        uint ts = QDateTime::currentDateTime().toTime_t();
                        uint sec = i.value().toInt();
                        timestamp.insert("start", QString(ts));
                        timestamp.insert("end", QString(ts + sec));

                        objts.insert("action", commands[it].toObject().value(key).toObject().value("action").toObject());
                        objts.insert("time", commands[it].toObject().value(key).toObject().value("time").toInt());
                        objts.insert("timestamp", timestamp);

                        node.insert(it, objts);
                        list_stopwatch.append(node);

                        qDebug() << node;
                    }
                    timer->start(1000);
                    connect(timer, SIGNAL(timeout()), this, SLOT(stopWatch()));
                    continue;
                }
            }
        }
    }
}

void SerialServer::prepareSerial(const QJsonObject node)
{
    QStringList write_node;
    write_node << "#0" << node[node.keys()[0]].toObject().value("range").toString() << "00" << node.keys()[0] << ":";
    serial->write(QString(write_node.join("")).toUtf8());
}

void SerialServer::stopWatch()
{
    for (quint8 i = 0; i < list_stopwatch.count(); i++) {
        QStringList nodes = list_stopwatch.at(i).keys();
        for (QString key: nodes) {
            if (list_stopwatch.at(i).value(key).toObject().value("time").toInt() > 0) {
                QJsonObject objs, node;
                objs.insert("time", (list_stopwatch.at(i).value(key).toObject().value("time").toInt() - 1));
                objs.insert("action", list_stopwatch.at(i).value(key).toObject().value("action").toObject());
                objs.insert("timestamp", list_stopwatch.at(i).value(key).toObject().value("timestamp").toObject());
                node.insert(key, objs);
                list_stopwatch.replace(i, node);
            } else {
                QJsonObject node;
                node.insert(key, list_stopwatch.at(i).value(key).toObject().value("action"));
                list_stopwatch.removeAt(i);
                prepareSerial(node);
            }
        }
    }

    if (list_stopwatch.count() == 0) {
        timer->stop();
    }
}
