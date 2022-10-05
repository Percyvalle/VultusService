#include <QCoreApplication>

#include "VultusServiceServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    VultusServiceServer server;

    return a.exec();
}
