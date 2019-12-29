#ifndef CAPTUREMANAGER_H
#define CAPTUREMANAGER_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
//#include <QPushButton>

#include <QCameraInfo>
//#include <QThread>
//#include <QCamera>

#include <opencv2/opencv.hpp>
//#include <opencv2/videoio.hpp>

//#include "CaptureEngine.h"

#define DEBUG_MODE

class CaptureManager : public QWidget
{
    Q_OBJECT

public:
    CaptureManager(QWidget *parent = nullptr);
    ~CaptureManager();

private:
//    CaptureEngine capture_engine;
//    QThread thread_for_capture;

    QGridLayout *layout = nullptr;
    QComboBox *combobox_list_cameras = nullptr;
    QLabel *debug_info = nullptr;
    QLabel *preview_camera_capture = nullptr;
//    QPushButton *start_capture_button = nullptr;
//    QPushButton *stop_capture_button = nullptr;

    quint32 width_preview = 0;
    quint32 height_preview = 0;

signals:
    void started(qint32 camera_id);
    void stopped();
    void captureChanged(qint32 camera_id);
    void frameCaptured(cv::Mat& frame);
public slots:
    void start();
    void stop();
    void displayFrame(cv::Mat& frame);
    void clearPreview();
};
#endif // CAPTUREMANAGER_H
