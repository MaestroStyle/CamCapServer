#include "CaptureEngine.h"
#include <QThread>

CaptureEngine::CaptureEngine(qint32 camera_id, QObject *parent) : QObject(parent)
{
    id_cur_camera = camera_id;
    QObject::connect(this, &CaptureEngine::nextFrameCaptured, this, &CaptureEngine::captureFrame);
}
CaptureEngine::~CaptureEngine(){
//    video_capture->release();
}
void CaptureEngine::startCapture(){
    process = true;
//    video_capture = new cv::VideoCapture(0/*id_cur_camera*/);
//    video_capture.open(-1);
////    if(video_capture->isOpened()){
//    if(video_capture.isOpened()){
//#ifdef DEBUG_MODE
//        qDebug(QString(QString(__FUNCTION__) + QString(" Error! Unable to open camera!")).toUtf8());
//#endif
//        stopCapture();
//        return;
//    }
//    video_capture->open(id_cur_camera);
    captureFrame();
}
void CaptureEngine::captureFrame(){
    if(process){
        cv::Mat frame;
//        video_capture->read(frame);
        video_capture.open(0);
        video_capture.read(frame);
        if(frame.empty()){
#ifdef DEBUG_MODE
            qDebug(QString(QString(__FUNCTION__) + QString(" Error! Blank frame grabbed!")).toUtf8());
#endif
            stopCapture();
            return;
        }
        emit frameCaptured(frame);
        emit nextFrameCaptured();
    }
//    video_capture.open(id_cur_camera);
//    cv::Mat frame;

//    while(process){
//        video_capture.read(frame);
//        if(frame.empty()){
//#ifdef DEBUG_MODE
//            qDebug(QString(QString(__FUNCTION__) + QString(" Error! Blank frame grabbed!")).toUtf8());
//#endif
//            emit frameCaptured(frame);
//            emit nextFrameCaptured();
//            stopCapture();
//            return;
//        }
//    }
}
void CaptureEngine::stopCapture(){
    process = false;
}
void CaptureEngine::setIdCamera(qint32 camera_id){
    id_cur_camera = camera_id;
//    video_capture->open(id_cur_camera);
    video_capture.open(id_cur_camera);
}
qint32 CaptureEngine::getIdCamera(){
    return id_cur_camera;
}
