#include "serialsimulator.h"

SerialSimulator::SerialSimulator(QObject *parent) : QObject(parent)
{

}

void SerialSimulator::CreateNodes()
{
    QString nodes_example = "$10001001:$13000002:$00001003:$17501004:$15801005:$13001006:";
    emit getSerial(nodes_example);
}
