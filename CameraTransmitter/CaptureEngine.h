#ifndef CAPTUREENGINE_H
#define CAPTUREENGINE_H

#include <QObject>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#define DEBUG_MODE

class CaptureEngine : public QObject
{
    Q_OBJECT
public:
    explicit CaptureEngine(qint32 camera_id, QObject *parent = nullptr);
    ~CaptureEngine();

//    cv::VideoCapture *video_capture = nullptr;
    cv::VideoCapture video_capture;
    bool process = false;
    qint32 id_cur_camera = 0;
signals:
    void frameCaptured(cv::Mat& frame);
    void nextFrameCaptured();
public slots:
    void startCapture();
    void captureFrame();
    void stopCapture();
    void setIdCamera(qint32 camera_id);
    qint32 getIdCamera();
signals:

};

#endif // CAPTUREENGINE_H
