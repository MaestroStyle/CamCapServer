#include "CameraCapture.h"

CameraCapture::CameraCapture(QWidget *parent)
    : QWidget(parent)
{
    layout = new QGridLayout(this);
    combobox_list_cameras = new QComboBox;
    layout->addWidget(combobox_list_cameras);

    width_preview = 320;
    height_preview = 240;
    preview_camera_capture = new QLabel;
    preview_camera_capture->setMinimumSize(width_preview, height_preview);
    layout->addWidget(preview_camera_capture);

    start_capture_button = new QPushButton("Start");
    stop_capture_button = new QPushButton("Stop");
    QObject::connect(start_capture_button, &QPushButton::clicked, this, &CameraCapture::startCapture);
    QObject::connect(stop_capture_button, &QPushButton::clicked, this, &CameraCapture::stopCapture);

    layout->addWidget(start_capture_button);
    layout->addWidget(stop_capture_button);

    QList<QCameraInfo> list_cameras_info = QCameraInfo::availableCameras();

    for (qint32 i = 0; i < list_cameras_info.count(); i++) {
            combobox_list_cameras->addItem(list_cameras_info[i].description());
    }

    QObject::connect(&capture_engine, &CaptureEngine::frameCaptured, this, &CameraCapture::displayFrame, Qt::DirectConnection);
    QObject::connect(&capture_engine, &CaptureEngine::processStopped, this, &CameraCapture::clearPreview, Qt::DirectConnection);
    QObject::connect(&thread_for_capture, &QThread::started, &capture_engine, &CaptureEngine::startCapture);
//    QString debug_text;
//    debug_text.append(QString("Count cameras: %1\n\r").arg(list_cameras_info.count()));
//    for (quint32 i = 0; i < list_cameras_info.count(); i++) {
//        QCamera cur_camera(list_cameras_info[i]);

//        QList<QSize> supported_resolution = cur_camera.supportedViewfinderResolutions();
//        QList<FrameRateRanges> supported_frame_rate = cur_camera.supportedViewfinderFrameRateRanges();
//        debug_text.append(QString(list_cameras_info[i].deviceName() + "\n\r"));
//        debug_text.append(QString(list_cameras_info[i].description() + "\n\r"));
//        debug_text.append(QString("\n\r"));
//    }

//    debug_info = new QLabel(this);
//    debug_info->setText(debug_text);
//    debug_info->show();
}

CameraCapture::~CameraCapture()
{
//    video_capture->release();
}
void CameraCapture::startCapture(){
   if(capture_engine.isProcess() || thread_for_capture.isRunning())
       return;
   qint32 camera_id = combobox_list_cameras->currentIndex();
#ifdef DEBUG_MODE
   qDebug(QString(combobox_list_cameras->currentText() + QString(", id = %1").arg(camera_id)).toUtf8());
#endif
   capture_engine.setIdCamera(camera_id);
   capture_engine.moveToThread(&thread_for_capture);

   thread_for_capture.start();
}
void CameraCapture::stopCapture(){
    if(capture_engine.isProcess()){
        capture_engine.stopCapture();
    }
}

void CameraCapture::displayFrame(cv::Mat& frame){
    if(frame.empty()){
#ifdef DEBUG_MODE
        qDebug(QString(QString(__FUNCTION__) + QString(" Received frame is empty!")).toUtf8());
#endif
        return;
    }
    cv::Mat copy_frame(frame);
    cv::resize(frame, copy_frame, cv::Size(320, 240));
    cv::cvtColor(copy_frame, copy_frame, cv::COLOR_BGR2RGB);
    QPixmap pix_frame;
    QImage img((uchar*) copy_frame.data, copy_frame.cols, copy_frame.rows, copy_frame.step, QImage::Format_RGB888);
    preview_camera_capture->setPixmap(QPixmap::fromImage(img, Qt::ColorOnly));
}
void CameraCapture::clearPreview(){
    preview_camera_capture->clear();
}


















