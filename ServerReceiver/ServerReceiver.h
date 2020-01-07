#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QGridLayout>

#include "ReceivePreview.h"
#include "ReceiverManager.h"
#include "Receiver.h"

class ServerReceiver : public QWidget
{
    Q_OBJECT

public:
    ServerReceiver(QWidget *parent = nullptr);

private:
    QGridLayout layout;

    ReceivePreview receive_preview;
    ReceiverManager receiver_manager;
};
#endif // SERVER_H
