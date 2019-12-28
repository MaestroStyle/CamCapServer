#include "CaptureEngine.h"
#include <QThread>

CaptureEngine::CaptureEngine(qint32 camera_id, QObject *parent) : QObject(parent)
{
    id_cur_camera = camera_id;
}
CaptureEngine::~CaptureEngine(){

}
bool CaptureEngine::isProcess(){
    return process;
}
void CaptureEngine::startCapture(){
    process = true;
    cv::Mat frame;
    video_capture.open(id_cur_camera + cv::CAP_ANY);
    if(!video_capture.isOpened()){
#ifdef DEBUG_MODE
            qDebug(QString(QString(__FUNCTION__) + QString(" Error! Unable to open camera!")).toUtf8());
#endif
        stopCapture();
    }
    while(process){
        video_capture.read(frame);
        if(frame.empty()){
#ifdef DEBUG_MODE
            qDebug(QString(QString(__FUNCTION__) + QString(" Error! Blank frame grabbed!")).toUtf8());
#endif
            stopCapture();
//            or continue;
        }
        emit frameCaptured(frame);
#ifdef DEBUG_MODE
        static quint32 num_frame_for = 0;
        qDebug(QString(QString(__FUNCTION__) + QString(" %1").arg(process)).toUtf8());
        qDebug(QString(QString(__FUNCTION__) + QString(" frame %1").arg(num_frame_for++)).toUtf8());
#endif
    }
    video_capture.release();
    emit processStopped();
    thread()->quit();
}
void CaptureEngine::stopCapture(){
    process = false;
}
void CaptureEngine::setIdCamera(qint32 camera_id){
    id_cur_camera = camera_id;
}
qint32 CaptureEngine::getIdCamera(){
    return id_cur_camera;
}
