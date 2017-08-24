#include "nodes.h"

Nodes::Nodes(QObject *parent) : QObject(parent)
{

}

void Nodes::addNodes(QString get_serial)
{
    QStringList read_list;
    read_list = get_serial.split(":");

    if (read_list.at((read_list.count() - 1)) == "")
        read_list.removeAt((read_list.count() - 1));

    for (const QString &str: read_list) {
        /*   s r  t  a
         * $ 1 30 01 006 :
         */

        double status = QString(str.mid(1, 1)).toInt();
        double range = QString(str.mid(2, 2)).toInt();
        double type = QString(str.mid(4, 2)).toInt();
        QString address = QString(str.mid(6, 3));

        QJsonObject new_object;
        new_object.insert("status", status);
        new_object.insert("range", range);
        new_object.insert("type", type);

        QJsonObject search = nodes_object[address].toObject();
        if (!search.isEmpty()) {
            if (nodes_object[address].toObject().value("range").toDouble() != range ||
                    nodes_object[address].toObject().value("status").toDouble() != status) {
                nodes_object[address] = new_object;
                nodes_update.append(address);
            }
        } else {
            nodes_object.insert(address, new_object);
            nodes_update.append(address);
        }
    }
    getNodesChanged();
}

void Nodes::getNodesChanged()
{
    if (nodes_update.size() > 0) {
        QJsonObject nodes_object_construct;
        QJsonObject nodes_object_send;
        QJsonArray nodes_array;
        for (const QString &node: nodes_update) {
            nodes_object_construct.insert(node, nodes_object[node]);
        }
        nodes_object_send.insert("Nodes", nodes_object_construct);
        nodes_array.push_back(nodes_object_send);

        nodes_update.clear();
        emit updateNodes(nodes_array);
    }
}

void Nodes::requireGetAllNodes(QTcpSocket *client)
{
    QJsonObject nodes_object_send;
    QJsonArray nodes_array;
    nodes_object_send.insert("Nodes", nodes_object);
    nodes_array.push_back(nodes_object_send);

    emit sendAllNodes(nodes_array, client);
}

void Nodes::writeSerialSimulate(QJsonObject node)
{
    QStringList keys = node.keys();
    for (const QString &n: keys) {
        if (node[n].toObject().value("range").toString() != nodes_object[n].toObject().value("range").toString()) {
            QJsonObject new_object;
            double status = nodes_object[n].toObject().value("status").toDouble();
            double range;
            if (node[n].toObject().value("range").type() == 2) {
                range = node[n].toObject().value("range").toDouble();
            } else {
                range = node[n].toObject().value("range").toString().toDouble();
            }
            double type = nodes_object[n].toObject().value("type").toDouble();

            new_object.insert("status", status);
            new_object.insert("range", range);
            new_object.insert("type", type);

            nodes_object[n] = new_object;
            nodes_update.append(n);
        }
    }
    getNodesChanged();
}
