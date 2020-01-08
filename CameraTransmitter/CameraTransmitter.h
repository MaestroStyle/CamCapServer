#ifndef CAMERATRANSMITTER_H
#define CAMERATRANSMITTER_H

#include <QWidget>
#include <QGridLayout>

#include "CaptureManager.h"
#include "CaptureEngine.h"
#include "TransmitManager.h"
#include "Transmitter.h"
#include "Coder.h"

class CameraTransmitter : public QWidget
{
    Q_OBJECT
public:
    explicit CameraTransmitter(QWidget *parent = nullptr);

private:
    QGridLayout layout;

    CaptureManager capture_manager;
    CaptureEngine capture_engine;
    TransmitManager transmit_manager;
    Transmitter transmitter;
    Coder coder;

    void closeEvent(QCloseEvent *event);

signals:
public slots:
    void startTransmit();
    void stopTransmit();
};

#endif // CAMERATRANSMITTER_H
