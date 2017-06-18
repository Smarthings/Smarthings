#include <QCoreApplication>
#include <QtCore>
#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QJsonArray nodes;
QJsonObject nodes_object;

void readJSON()
{
    QByteArray val;
    QFile file("/tmp/nodes.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    val = file.readAll();
    file.close();

    QJsonParseError errorParse;
    QJsonDocument doc = QJsonDocument::fromJson(val, &errorParse);

    qDebug() << "Status parsing: " << errorParse.error;

    QJsonValue value = doc.array().at(0);
    QJsonObject object = value.toObject();
    QJsonValue nodes_value = object.value("Nodes");
    QJsonObject nodes = nodes_value.toObject();

    QStringList nodes_keys = nodes.keys();
    QJsonObject n_object;
    QJsonObject search = nodes["0006"].toObject();
    /*if (!search.isEmpty()) {
        n_object.insert("status", "0002");
        nodes["0006"] = n_object;
    } else {
        qDebug() << "Não encontrado";
    }*/

    for (int i = 0; i < nodes_keys.count(); i++) {
        qDebug() << "Node:" << nodes_keys.at(i) << "status:" << nodes.value(nodes_keys.at(i)).toObject().value("status").toString();
    }
}

void addJSON()
{
    QString str = "$00010001:$00000002:$00000003:$00010004:$00010005";
    QStringList str_split = str.split(":");

    QJsonObject json_object;
    for (const QString &s: str_split) {
        QJsonObject new_object;
        new_object.insert("status", s.mid(1, 4));
        json_object.insert(QString(s.mid(5, 4)), new_object);
    }

    QString name_node = "0006";
    QJsonObject search = json_object[name_node].toObject();

    QJsonObject new_object;
    new_object.insert("status", "0000");
    qDebug() << "search: " << search.isEmpty();
    if (!search.isEmpty()) {
        json_object[name_node] = new_object;
    } else {
        json_object.insert(name_node, new_object);
    }

    /*if (search.isObject()) {
        qDebug() << "Existe: " << search.toObject().value("status");
    } else {
        qDebug() << "Não existe";
    }*/

    QJsonObject final_object;
    final_object.insert("Nodes", json_object);

    QJsonArray array;
    array.push_back(final_object);

    QJsonDocument doc(array);

    QFile file("/tmp/nodes.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(doc.toJson());
    file.close();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    addJSON();
    readJSON();

    return a.exec();
}
