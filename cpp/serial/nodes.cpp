#include "nodes.h"

Nodes::Nodes(QJsonArray *var_nodes, QObject *parent) : QObject(parent)
{
    nodes_v = *var_nodes;
}

void Nodes::addNodes(QString get_serial)
{
    QStringList read_list;
    read_list = get_serial.split(":");

    if (read_list.at((read_list.count() - 1)) == "")
        read_list.removeAt((read_list.count() - 1));

    foreach (const QString &node, read_list) {
        QStringList split_node;
        split_node.append(node.mid(1, 4));
        split_node.append(node.mid(5, 4));
        addNodeJSON(&split_node);
    }
    getNodesChanged();
}

void Nodes::addNodeJSON(QStringList *node)
{
    QJsonObject node_object {};
    if (searchNode(node->at(1), node->at(0)) == false) {
        if (node->count() == 2) {
            node_object.insert("node", node->at(1));
            node_object.insert("status", node->at(0));
            nodes_v.append(node_object);
            update_nodes.append(nodes_v.count() -1);
        }
    }
}

bool Nodes::searchNode(QString node, QString status)
{
    for (int i = 0; i < nodes_v.size(); i++) {
        QJsonObject object = nodes_v[i].toObject();
        if (object["node"] == node) {
            if (object["status"] != status) {
                QJsonObject newNode;
                newNode.insert("node", node);
                newNode.insert("status", status);
                nodes_v.replace(i, newNode);
                update_nodes.append(i);
            }
            return true;
        }
    }
    return false;
}

void Nodes::getNodesChanged()
{
    if (update_nodes.size() > 0) {
        QJsonArray send_nodes;
        for (int i = 0; i < update_nodes.size(); i++) {
            send_nodes.append(nodes_v.at(update_nodes.at(i)));
        }
        update_nodes.clear();
        emit updateNodes(send_nodes);
    }
}
