#include <QCoreApplication>

#include "./socketclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SocketClient client;

    return a.exec();
}
