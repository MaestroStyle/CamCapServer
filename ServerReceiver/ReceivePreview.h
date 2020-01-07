#ifndef RECEIVEPREVIEW_H
#define RECEIVEPREVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include <opencv2/opencv.hpp>

class ReceivePreview : public QWidget
{
    Q_OBJECT
public:
    explicit ReceivePreview(QWidget *parent = nullptr);

    bool isProcess();
    void setSizePreview(QSize size);
    void setSizePreview(quint32 width, quint32 height);
    QSize getSizePreview();
private:
    QGridLayout layout;
    QLabel preview_display;

    quint32 width_preview = 0;
    quint32 height_preview = 0;
    bool process = false;

signals:
    void started();
    void stopped();
public slots:
    void start();
    void stop();
    void displayFrame(cv::Mat& frame);
    void clearPreview();
};

#endif // RECEIVEPREVIEW_H
