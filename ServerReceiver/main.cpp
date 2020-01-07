#include "ServerReceiver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerReceiver w;
    w.show();
    return a.exec();
}
