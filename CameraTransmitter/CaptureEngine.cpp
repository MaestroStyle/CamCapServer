#include "CaptureEngine.h"

CaptureEngine::CaptureEngine(QObject *parent) : QObject(parent){}
CaptureEngine::~CaptureEngine(){}
bool CaptureEngine::isProcess(){
    return process;
}
void CaptureEngine::start(qint32 camera_id){
    process = true;
    video_capture.open(camera_id + cv::CAP_ANY);
    if(!video_capture.isOpened()){
#ifdef DEBUG_MODE
            qDebug(QString(QString(__FUNCTION__) + QString(" Error! Unable to open camera!")).toUtf8());
#endif
        stop();
        return;
    }
//    extern void capturing();
    QtConcurrent::run(this, &CaptureEngine::capturing);
}
void CaptureEngine::capturing(){
    cv::Mat frame;
    while(process){
        video_capture.read(frame);
        if(frame.empty()){
#ifdef DEBUG_MODE
            qDebug(QString(QString(__FUNCTION__) + QString(" Error! Blank frame grabbed!")).toUtf8());
#endif
//            stop();
//            break;
            continue;
        }
        emit frameCaptured(frame);
#ifdef DEBUG_MODE
        static quint32 num_frame_for = 0;
        qDebug(QString(QString(__FUNCTION__) + QString(" %1").arg(process)).toUtf8());
        qDebug(QString(QString(__FUNCTION__) + QString(" frame %1").arg(num_frame_for++)).toUtf8());
#endif
    }
    video_capture.release();
    emit stopped();

    return; //thread()->quit();
}
void CaptureEngine::stop(){
    process = false;
}
void CaptureEngine::changeCapture(qint32 camera_id){
    stop();
    thread()->msleep(200);
    start(camera_id);
}
//void CaptureEngine::setIdCamera(qint32 camera_id){
//    id_cur_camera = camera_id;
//}
//qint32 CaptureEngine::getIdCamera(){
//    return id_cur_camera;
//}
