#include "udpserver.h"

UdpServer::UdpServer(QObject *parent) : QObject(parent)
{
    udpServer = new QUdpSocket(this);
    this->bind();
}

void UdpServer::bind()
{
    udpServer->bind(QHostAddress::Broadcast, 9932);
    connect(udpServer, SIGNAL(readyRead()), this, SLOT(getClient()));
    connect(this, SIGNAL(hostConnect(QHostAddress)), this, SLOT(sendMessage(QHostAddress)));
}

void UdpServer::getClient()
{
    while (udpServer->hasPendingDatagrams()) {
        QByteArray buffer;
        QHostAddress sender;

        buffer.resize(udpServer->pendingDatagramSize());
        udpServer->readDatagram(buffer.data(), buffer.size(), &sender);
        emit hostConnect(sender);
    }
}

void UdpServer::sendMessage(QHostAddress ip_address)
{
    QByteArray datagram = "Message from Server";
    udpServer->writeDatagram(datagram.data(), datagram.size(), ip_address, 9933);
}
