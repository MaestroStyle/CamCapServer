#include "CameraCapture.h"

CameraCapture::CameraCapture(QWidget *parent)
    : QWidget(parent)
{
    video_capture = new cv::VideoCapture();
    quint32 success_open[100];
    for (quint32 i = 0; i < 100; i++) {
        if(video_capture->open(i)){
            success_open[i] = 1;
            qDebug(QString("Succec_open[%1]").arg(i).toUtf8());
        } else success_open[i] = 0;
    }

}

CameraCapture::~CameraCapture()
{
    video_capture->release();
}

