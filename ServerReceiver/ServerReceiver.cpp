#include "ServerReceiver.h"

ServerReceiver::ServerReceiver(QWidget *parent)
    : QWidget(parent)
{
    setLayout(&layout);
    layout.addWidget(&receive_preview);
    layout.addWidget(&receiver_manager);

    QObject::connect(&receiver_manager, &ReceiverManager::started, &receiver, &Receiver::start, Qt::DirectConnection);
    QObject::connect(&receiver_manager, &ReceiverManager::stopped, &receiver, &Receiver::stop, Qt::DirectConnection);
    QObject::connect(&receiver, &Receiver::frameReceived, &decoder, &Decoder::decode, Qt::DirectConnection);
    QObject::connect(&decoder, &Decoder::decoded, &receive_preview, &ReceivePreview::displayFrame);
    QObject::connect(&receiver, &Receiver::receiveStopped, &receive_preview, &ReceivePreview::clearPreview, Qt::DirectConnection);
    QObject::connect(&receiver, &Receiver::aborted, &receiver_manager, &ReceiverManager::error, Qt::DirectConnection);
    receiver.moveToThread(&thread_receive);
}
void ServerReceiver::closeEvent(QCloseEvent *event){
    if(receiver_manager.isProcess()){
        receiver_manager.stop();
    }
    if(thread_receive.isRunning())
        thread_receive.quit();
}
