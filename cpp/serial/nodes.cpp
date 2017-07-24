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

        QString status = QString(str.mid(1, 1));
        QString range = QString(str.mid(2, 2));
        QString type = QString(str.mid(4, 2));
        QString address = QString(str.mid(6, 3));

        /*QString node = QString(str.mid(5, 4));
        QString status = QString(str.mid(4, 1));
        QString range = QString(str.mid(1, 3));*/

        QJsonObject new_object;
        new_object.insert("status", status);
        new_object.insert("range", range);
        new_object.insert("type", type);

        QJsonObject search = nodes_object[address].toObject();
        if (!search.isEmpty()) {
            if (nodes_object[address].toObject().value("range").toString() != range ||
                    nodes_object[address].toObject().value("status").toString() != status) {
                nodes_object[address] = new_object;
                nodes_update.append(address);
            }
        } else {
            nodes_object.insert(address, new_object);
            nodes_update.append(address);
        }

        /*QJsonObject search = nodes_object[node].toObject();
        if (!search.isEmpty()) {
            if (nodes_object[node].toObject().value("status").toString() != status ||
                    nodes_object[node].toObject().value("range").toString() != range) {
                nodes_object[node] = new_object;
                nodes_update.append(node);
            }
        } else {
            nodes_object.insert(node, new_object);
            nodes_update.append(node);
        }*/
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

void Nodes::requireGetAllNodes()
{
    QJsonObject nodes_object_send;
    QJsonArray nodes_array;
    nodes_object_send.insert("Nodes", nodes_object);
    nodes_array.push_back(nodes_object_send);

    //qDebug() << nodes_array;
    emit sendAllNodes(nodes_array);
}
