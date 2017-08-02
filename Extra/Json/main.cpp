#include <QCoreApplication>
#include "./jsontest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    JsonTest json;

    return a.exec();
}
