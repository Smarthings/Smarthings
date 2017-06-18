#ifndef NODES_H
#define NODES_H

#include <QObject>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QVector>

class Nodes : public QObject
{
    Q_OBJECT
public:
    explicit Nodes(QObject *parent = 0);

signals:
    void updateNodes(const QJsonArray nodes_array);
    void sendAllNodes(const QJsonArray nodes_array);

public slots:
    void addNodes(QString get_serial);
    void requireGetAllNodes();

private:
    QJsonArray sendNodes(QJsonArray nodes_send);
    void getNodesChanged();

protected:
    QJsonObject nodes_object;
    QStringList nodes_update;
};

#endif // NODES_H
