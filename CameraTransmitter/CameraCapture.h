#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

#include <QCameraInfo>
#include <QThread>
//#include <QCamera>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include "CaptureEngine.h"

#define DEBUG_MODE

class CameraCapture : public QWidget
{
    Q_OBJECT

public:
    CameraCapture(QWidget *parent = nullptr);
    ~CameraCapture();

private:
    CaptureEngine capture_engine;
    QThread thread_for_capture;

    QGridLayout *layout = nullptr;
    QComboBox *combobox_list_cameras = nullptr;
    QLabel *debug_info = nullptr;
    QLabel *preview_camera_capture = nullptr;
    QPushButton *start_capture_button = nullptr;
    QPushButton *stop_capture_button = nullptr;

    quint32 width_preview = 0;
    quint32 height_preview = 0;

signals:
    void frameCaptured(cv::Mat& frame);
public slots:
    void startCapture();
    void stopCapture();
    void displayFrame(cv::Mat& frame);
    void clearPreview();
};
#endif // CAMERACAPTURE_H
