#include "ServerReceiver.h"

ServerReceiver::ServerReceiver(QWidget *parent)
    : QWidget(parent)
{
    setLayout(&layout);
    layout.addWidget(&receive_preview);
    layout.addWidget(&receiver_manager);
}

