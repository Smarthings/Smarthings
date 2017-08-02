#ifndef JSONTEST_H
#define JSONTEST_H

#include <QObject>
#include <QDebug>

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>

#include <QDateTime>

class JsonTest : public QObject
{
    Q_OBJECT
public:
    explicit JsonTest(QObject *parent = nullptr);

signals:

public slots:
};

#endif // JSONTEST_H
