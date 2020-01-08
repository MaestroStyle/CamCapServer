#include "ServerReceiver.h"

ServerReceiver::ServerReceiver(QWidget *parent)
    : QWidget(parent)
{
    setLayout(&layout);
    layout.addWidget(&receive_preview);
    layout.addWidget(&receiver_manager);

    QObject::connect(&receiver_manager, &ReceiverManager::started, &receiver, &Receiver::start);
    QObject::connect(&receiver_manager, &ReceiverManager::stopped, &receiver, &Receiver::stop);
    QObject::connect(&receiver, &Receiver::frameReceived, &decoder, &Decoder::decode);
    QObject::connect(&decoder, &Decoder::decoded, &receive_preview, &ReceivePreview::displayFrame);
    QObject::connect(&receiver, &Receiver::receiveStopped, &receive_preview, &ReceivePreview::clearPreview);
    QObject::connect(&receiver, &Receiver::aborted, &receiver_manager, &ReceiverManager::error);
}

