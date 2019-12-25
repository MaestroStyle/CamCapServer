#include "CameraCapture.h"

CameraCapture::CameraCapture(QWidget *parent)
    : QWidget(parent)
{
//    video_capture = new cv::VideoCapture();
//    quint32 success_open[100];
//    for (quint32 i = 0; i < 100; i++) {
//        if(video_capture->open(i)){
//            success_open[i] = 1;
//            qDebug(QString("Success_open[%1]").arg(i).toUtf8());
//        } else success_open[i] = 0;
//    }
    layout = new QGridLayout(this);
    combobox_list_cameras = new QComboBox;
    layout->addWidget(combobox_list_cameras);

    preview_camera_capture = new QLabel;
    layout->addWidget(preview_camera_capture);

    QList<QCameraInfo> list_cameras_info = QCameraInfo::availableCameras();

    for (qint32 i = 0; i < list_cameras_info.count(); i++) {
            combobox_list_cameras->addItem(list_cameras_info[i].description());
    }


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
void CameraCapture::startCapture(qint32 camera_id){
   thread_for_capture = new QThread;
   capture_engine = new CaptureEngine(camera_id);
   capture_engine->moveToThread(thread_for_capture);

   QObject::connect(capture_engine, &CaptureEngine::frameCaptured, this, &CameraCapture::displayFrame, Qt::DirectConnection);
   QObject::connect(thread_for_capture, &QThread::started, capture_engine, &CaptureEngine::startCapture);
   QObject::connect(this, &CameraCapture::captureStopped, capture_engine, &CaptureEngine::stopCapture, Qt::DirectConnection);

   thread_for_capture->start();
}

void CameraCapture::displayFrame(cv::Mat& frame){
    cv::Mat copy_frame(frame);
    cv::resize(frame, copy_frame, cv::Size(320, 240));
    cv::cvtColor(copy_frame, copy_frame, cv::COLOR_BGR2RGB);
    QPixmap pix_frame;
//    pix_frame.loadFromData(frame.data, frame.total() * 3, "BMP");
    QImage img((uchar*) copy_frame.data, copy_frame.cols, copy_frame.rows, copy_frame.step, QImage::Format_RGB888);
//    pix_frame.fromImage(img);
    preview_camera_capture->setPixmap(/*pix_frame*/QPixmap::fromImage(img, Qt::ColorOnly));
}


















