#ifndef NODES_H
#define NODES_H

#include <QObject>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

class Nodes : public QObject
{
    Q_OBJECT
public:
    explicit Nodes(QJsonArray *var_nodes, QObject *parent = 0);

signals:

public slots:
    void addNodes(QString get_serial);

private:
    void addNodeJSON(QStringList *node);
    int searchNode(QString node, QString status);

protected:
    QJsonArray nodes_v;
};

#endif // NODES_H
