#include "jsontest.h"

JsonTest::JsonTest(QObject *parent) : QObject(parent)
{
    QJsonObject objs, node;
    int timestamp = QDateTime::currentDateTime().toTime_t();
    objs.insert("timestamp", timestamp + 500);

    qDebug() << timestamp << objs;
}
