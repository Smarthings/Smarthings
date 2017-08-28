#include "serialserver.h"

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SerialServer::SerialServer(bool simulate, QString port, QObject *parent) :
    QObject(parent)
{
    v_simulate = simulate;
    if (v_simulate)
        return;
    v_port = port;

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
    if (v_port.isNull())
        serial->setPortName(ports.at(4));
    else
        serial->setPortName(v_port);
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
    if (v_simulate)
        return;
    serial->write(data);
}

void SerialServer::writeSerial(const QJsonObject nodes)
{
    if (v_simulate)
        return;
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
                    if (commands[it].toObject().value(key).toObject().contains("time") &&
                        commands[it].toObject().value(key).toObject().contains("action")) {
                    QJsonObject timestamp, objts, node;

                    //int ts = QDateTime::currentDateTime().toUTC().toTime_t();
                    int sec = commands[it].toObject().value(key).toObject().value("time").toInt();

                    objts.insert("to_range", commands[it].toObject().value(key).toObject().value("action").toObject().value("range").toString());
                    //objts.insert("end", ts + sec);
                    objts.insert("time", sec);

                    node.insert(it, objts);
                    list_stopwatch.append(node);

                    QJsonObject node_send;
                    QJsonArray node_array;

                    node_send.insert("Stopwatch", node);
                    node_array.append(node_send);
                    emit sendStopwatchCommand(node_array);

                    connect(timer, SIGNAL(timeout()), this, SLOT(stopWatch()));
                    if (!timer->isActive()) {
                        timer->start(1000);
                    }
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
    if (!v_simulate)
        serial->write(QString(write_node.join("")).toUtf8());
    else
        emit writeSerialSimulate(node);
}

void SerialServer::stopWatch()
{
    for (quint8 i = 0; i < list_stopwatch.count(); i++) {
        QStringList nodes = list_stopwatch.at(i).keys();
        for (QString key: nodes) {
            if (list_stopwatch.at(i).value(key).toObject().value("time").toDouble() > 0) {
                double v_time = list_stopwatch.at(i).value(key).toObject().value("time").toDouble() -1;
                QJsonObject values, node;
                values.insert("time", v_time);
                values.insert("to_range", list_stopwatch.at(i).value(key).toObject().value("to_range").toString());
                node.insert(key, values);

                list_stopwatch.replace(i, node);
            } else {
                QJsonObject range, node;
                range.insert("range", list_stopwatch.at(i).value(key).toObject().value("to_range").toString());
                node.insert(key, range);
                list_stopwatch.removeAt(i);
                prepareSerial(node);
            }
        }
    }

    if (list_stopwatch.count() == 0) {
        timer->stop();
    }
}

void SerialServer::requireGetAllStopwatch(QTcpSocket *client)
{
    for (quint8 i = 0; i < list_stopwatch.count(); i++) {
        QStringList nodes = list_stopwatch.at(i).keys();
        for (QString key: nodes) {
            QJsonObject node, stopwatch;
            QJsonArray node_array;
            node.insert(key, list_stopwatch.at(i).value(key));
            stopwatch.insert("Stopwatch", node);
            node_array.append(stopwatch);
            emit sendStopwatchToClient(node_array, client);
        }
    }
}
