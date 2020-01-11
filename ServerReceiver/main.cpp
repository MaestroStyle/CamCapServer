#include "ServerReceiver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("CamCapServer");
    QCoreApplication::setApplicationName("ServerReceiver");
    ServerReceiver w;
    w.show();
    return a.exec();
}
