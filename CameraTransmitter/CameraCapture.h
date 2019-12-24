#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <QWidget>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

class CameraCapture : public QWidget
{
    Q_OBJECT

public:
    CameraCapture(QWidget *parent = nullptr);
    ~CameraCapture();

private:
    cv::VideoCapture *video_capture = nullptr;
};
#endif // CAMERACAPTURE_H
