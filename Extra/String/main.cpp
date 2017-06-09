#include <QCoreApplication>
#include <QtCore>
#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QJsonArray nodes;

void readJson()
{
    foreach (QJsonValue obj, nodes) {
        qDebug() << obj.toObject();
    }
    qDebug() << "";
}

int searchJSON(QString node, QString status)
{
    for (int i = 0; i < nodes.size(); i++) {
        QJsonObject obj = nodes[i].toObject();
        if (obj["node"] == node) {
            QJsonObject newNode;
            newNode.insert("node", node);
            newNode.insert("status", status);
            nodes.replace(i, newNode);

            return 1;
        }
    }
    return 0;
}

void listJSON(QStringList *list)
{
    QJsonObject object{};
    if (list->count() == 2) {
        if (searchJSON(list->at(1), list->at(0)) == 0) {
            object.insert("node", list->at(1));
            object.insert("status", list->at(0));
            nodes.append(object);
        }
    }
}

void list(QStringList *list)
{
    int count = (list->count()) -1;
    if (list->at(count) == "")
        list->removeAt(count);

    count = (list->count() -1);

    foreach (const QString &str, *list) {
        QStringList l;
        l.append(str.mid(1, 4));
        l.append(str.mid(5, 4));
        listJSON(&l);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString read_serial;
    QStringList read_list;

    read_serial = "$00000002:$00000003:$00000004:$00100005:$00100006:";
    //qDebug() << read_serial;

    read_list = read_serial.split(":");
    list(&read_list);
    read_list.clear();

    readJson();

    read_serial = "$00100002:$00000003:$00100004:$00000005:$00100006:";
    //qDebug() << read_serial;

    read_list = read_serial.split(":");
    list(&read_list);
    read_list.clear();

    readJson();

    read_serial = "$00100003:$00000006:$00000002:";
    //qDebug() << read_serial;

    read_list = read_serial.split(":");
    list(&read_list);
    read_list.clear();

    readJson();




    return a.exec();
}
