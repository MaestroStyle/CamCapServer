#ifndef CAMERATRANSMITTER_H
#define CAMERATRANSMITTER_H

#include <QWidget>
#include <QGridLayout>
#include <QThread>

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

    QThread thread_transmit;

    void closeEvent(QCloseEvent *event);

signals:
    void frameReady(QByteArray encoded_frame);
public slots:
    void startTransmit();
    void stopTransmit();
    void preparationForTransmit(QByteArray frame_encoded);
};

#endif // CAMERATRANSMITTER_H
