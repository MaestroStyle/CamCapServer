#ifndef CODER_H
#define CODER_H

#include <QObject>
#include <QDebug>
#include <opencv2/opencv.hpp>

class Coder : public QObject
{
    Q_OBJECT

public:
    Coder();
signals:
    void encoded(QByteArray data);
public slots:
    void encode(cv::Mat& frame);
};

#endif // CODER_H
