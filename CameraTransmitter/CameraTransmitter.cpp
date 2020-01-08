#include "CameraTransmitter.h"

CameraTransmitter::CameraTransmitter(QWidget *parent) : QWidget(parent)
{
    setLayout(&layout);
    layout.addWidget(&capture_manager);
    layout.addWidget(&transmit_manager);

    QObject::connect(&capture_manager, &CaptureManager::started, &capture_engine, &CaptureEngine::start);
    QObject::connect(&capture_manager, &CaptureManager::stopped, &capture_engine, &CaptureEngine::stop);
    QObject::connect(&capture_manager, &CaptureManager::captureChanged, &capture_engine, &CaptureEngine::changeCapture);
    QObject::connect(&capture_engine, &CaptureEngine::frameCaptured, &capture_manager, &CaptureManager::displayFrame, Qt::DirectConnection);
    QObject::connect(&capture_engine, &CaptureEngine::stopped, &capture_manager, &CaptureManager::clearPreview, Qt::DirectConnection);
    QObject::connect(&transmit_manager, &TransmitManager::started, &transmitter, &Transmitter::start);
    QObject::connect(&transmit_manager, &TransmitManager::stopped, &transmitter, &Transmitter::stop);
    QObject::connect(&transmitter, &Transmitter::aborted, &transmit_manager, &TransmitManager::error);
    QObject::connect(&transmitter, &Transmitter::started, this, &CameraTransmitter::startTransmit);
    QObject::connect(&transmitter, &Transmitter::stopped, this, &CameraTransmitter::stopTransmit);

    capture_manager.start();
}
void CameraTransmitter::closeEvent(QCloseEvent *event){
    if(capture_manager.isProcess()){
        capture_manager.stop();
    }
    if(transmit_manager.isProcess()){
        transmit_manager.stop();
    }
}
void CameraTransmitter::startTransmit(){
    QObject::connect(&capture_engine, &CaptureEngine::frameCaptured, &transmitter, &Transmitter::transmitFrame, Qt::DirectConnection);
}
void CameraTransmitter::stopTransmit(){
    QObject::disconnect(&capture_engine, &CaptureEngine::frameCaptured, &transmitter, &Transmitter::transmitFrame);
}
