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
    QObject::connect(&transmit_manager, &TransmitManager::started, &transmitter, &Transmitter::start, Qt::DirectConnection);
    QObject::connect(&transmit_manager, &TransmitManager::stopped, &transmitter, &Transmitter::stop, Qt::DirectConnection);
    QObject::connect(&transmitter, &Transmitter::aborted, &transmit_manager, &TransmitManager::error, Qt::DirectConnection);
    QObject::connect(&transmitter, &Transmitter::started, this, &CameraTransmitter::startTransmit, Qt::DirectConnection);
    QObject::connect(&transmitter, &Transmitter::stopped, this, &CameraTransmitter::stopTransmit, Qt::DirectConnection);
    QObject::connect(&coder, &Coder::encoded, this, &CameraTransmitter::preparationForTransmit);
    QObject::connect(this, &CameraTransmitter::frameReady, &transmitter, &Transmitter::transmitFrame);
    transmitter.moveToThread(&thread_transmit);
    thread_transmit.start();
    capture_manager.start();
}
void CameraTransmitter::closeEvent(QCloseEvent *event){
    if(capture_manager.isProcess()){
        capture_manager.stop();
    }
    if(transmit_manager.isProcess()){
        transmit_manager.stop();
    }
    if(thread_transmit.isRunning())
        thread_transmit.quit();
}
void CameraTransmitter::startTransmit(){
    QObject::connect(&capture_engine, &CaptureEngine::frameCaptured, &coder, &Coder::encode, Qt::DirectConnection);
}
void CameraTransmitter::stopTransmit(){
    QObject::disconnect(&capture_engine, &CaptureEngine::frameCaptured, &coder, &Coder::encode);
}
void CameraTransmitter::preparationForTransmit(QByteArray frame_encoded){
    if(!transmitter.isReadyTransmit())
        return;
    emit frameReady(frame_encoded);
}
