#include "ReceivePreview.h"

ReceivePreview::ReceivePreview(QWidget *parent) : QWidget(parent)
{
    setLayout(&layout);
    layout.addWidget(&preview_display);
    width_preview = 320;
    height_preview = 240;
    preview_display.setMinimumSize(static_cast<int>(width_preview), static_cast<int>(height_preview));
    preview_display.setStyleSheet("QLabel {"
                                  "border-style: solid;"
                                  "border-width: 1px;"
                                  "border-color: black; "
                                  "}");
}
void ReceivePreview::start(){
    process = true;
    emit started();
}
void ReceivePreview::stop(){
    process = false;
    emit stopped();
}
void ReceivePreview::displayFrame(cv::Mat& frame){
    if(frame.empty()){
        return;
    }
    cv::Mat copy_frame(frame);
    cv::resize(frame, copy_frame, cv::Size(static_cast<int>(width_preview), static_cast<int>(height_preview)));
    cv::cvtColor(copy_frame, copy_frame, cv::COLOR_BGR2RGB);
    QPixmap pix_frame;
    QImage img(static_cast<uchar*>(copy_frame.data),
               copy_frame.cols,
               copy_frame.rows,
               static_cast<int>(copy_frame.step),
               QImage::Format_RGB888);
    preview_display.setPixmap(QPixmap::fromImage(img, Qt::ColorOnly));
}
void ReceivePreview::clearPreview(){
    preview_display.clear();
}
bool ReceivePreview::isProcess(){
    return process;
}
void ReceivePreview::setSizePreview(QSize size){
    if(size.width() < 0 || size.height() < 0)
        return;
    width_preview = static_cast<quint32>(size.width());
    height_preview = static_cast<quint32>(size.height());
    preview_display.setMinimumSize(static_cast<int>(width_preview), static_cast<int>(height_preview));
}
void ReceivePreview::setSizePreview(quint32 width, quint32 height){
    width_preview = width;
    height_preview = height;
    preview_display.setMinimumSize(static_cast<int>(width_preview), static_cast<int>(height_preview));
}
QSize ReceivePreview::getSizePreview(){
    return QSize(static_cast<int>(width_preview), static_cast<int>(height_preview));
}
