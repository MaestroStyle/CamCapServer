#include "CameraTransmitter.h"

CameraTransmitter::CameraTransmitter(QWidget *parent) : QWidget(parent)
{
    setLayout(&layout);
    layout.addWidget(&capture_manager);

    QObject::connect(&capture_manager, &CaptureManager::started, &capture_engine, &CaptureEngine::start);
    QObject::connect(&capture_manager, &CaptureManager::stopped, &capture_engine, &CaptureEngine::stop);
    QObject::connect(&capture_manager, &CaptureManager::captureChanged, &capture_engine, &CaptureEngine::changeCapture);
    QObject::connect(&capture_engine, &CaptureEngine::frameCaptured, &capture_manager, &CaptureManager::displayFrame, Qt::DirectConnection);
    QObject::connect(&capture_engine, &CaptureEngine::stopped, &capture_manager, &CaptureManager::clearPreview, Qt::DirectConnection);

    capture_manager.start();
}
