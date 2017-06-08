#include <QCoreApplication>
#include <QtCore>
#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QJsonArray nodes;

void listJSON(QStringList *list)
{
    QJsonObject object{};
    if (list->count() == 2) {
        object.insert(list->at(1), list->at(0));
    }
    nodes.append(object);
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
    qDebug() << read_serial;

    read_list = read_serial.split(":");
    list(&read_list);
    //qDebug() << read_list;

    qDebug() << nodes;

    return a.exec();
}
