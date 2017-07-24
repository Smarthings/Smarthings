#include <QCoreApplication>

#include "./socketserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SocketServer socket;

    return a.exec();
}
