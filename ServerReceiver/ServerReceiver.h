#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QGridLayout>

#include "ReceivePreview.h"
#include "ReceiverManager.h"
#include "Receiver.h"
#include "Decoder.h"

#include <QThread>

class ServerReceiver : public QWidget
{
    Q_OBJECT

public:
    ServerReceiver(QWidget *parent = nullptr);

private:
    void closeEvent(QCloseEvent *event);

    QGridLayout layout;

    ReceivePreview receive_preview;
    ReceiverManager receiver_manager;
    Receiver receiver;
    Decoder decoder;

    QThread thread_receive;
};
#endif // SERVER_H
