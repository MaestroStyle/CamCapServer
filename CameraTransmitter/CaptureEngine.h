#ifndef CAPTUREENGINE_H
#define CAPTUREENGINE_H

#include <QObject>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include <QtConcurrent/QtConcurrent>
//#define DEBUG_MODE

class CaptureEngine : public QObject
{
    Q_OBJECT
public:
    explicit CaptureEngine(QObject *parent = nullptr);
    ~CaptureEngine();
    bool isProcess();
private:
    cv::VideoCapture video_capture;
    bool process = false;
//    qint32 id_cur_camera = 0;
signals:
    void frameCaptured(cv::Mat& frame);
    void stopped();
public slots:
    void start(qint32 camera_id);
    void stop();
    void changeCapture(qint32 camera_id);
private slots:
    void capturing();
//    void setIdCamera(qint32 camera_id);
//    qint32 getIdCamera();
signals:

};

#endif // CAPTUREENGINE_H
