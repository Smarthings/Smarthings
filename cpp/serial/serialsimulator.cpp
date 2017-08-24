#include "serialsimulator.h"

SerialSimulator::SerialSimulator(QObject *parent) : QObject(parent)
{

}

void SerialSimulator::CreateNodes()
{
    QString nodes_example = "$10001001:$13002002:$00002003:$17502004:$15802005:$19901006:";
    emit getSerial(nodes_example);
}
