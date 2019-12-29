#ifndef CAMERATRANSMITTER_H
#define CAMERATRANSMITTER_H

#include <QWidget>
#include <QGridLayout>

#include "CaptureManager.h"
#include "CaptureEngine.h"
#include "TransmitManager.h"
#include "Transmitter.h"

class CameraTransmitter : public QWidget
{
    Q_OBJECT
public:
    explicit CameraTransmitter(QWidget *parent = nullptr);

private:
    QGridLayout layout;

    CaptureManager capture_manager;
    CaptureEngine capture_engine;

signals:

};

#endif // CAMERATRANSMITTER_H
