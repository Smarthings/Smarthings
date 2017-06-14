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
    explicit Nodes(QJsonArray *var_nodes, QObject *parent = 0);

signals:
    void updateNodes(const QJsonArray nodes_v);

public slots:
    void addNodes(QString get_serial);

private:
    void addNodeJSON(QStringList *node);
    bool searchNode(QString node, QString status);
    void getNodesChanged();

protected:
    QJsonArray nodes_v;
    QVector<qint8> update_nodes;
};

#endif // NODES_H
