#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>

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
    CaptureEngine *video_capture = nullptr;
    QThread *thread_for_capture = nullptr;

    QGridLayout *layout = nullptr;
    QComboBox *combobox_list_cameras = nullptr;
    QLabel *debug_info = nullptr;
    QLabel *preview_camera_capture = nullptr;

signals:
    void frameCaptured(cv::Mat& frame);
    void captureStopped();
public slots:
    void startCapture(qint32 camera_id);
    void displayFrame(cv::Mat& frame);
};
#endif // CAMERACAPTURE_H
