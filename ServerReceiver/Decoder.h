#ifndef DECODER_H
#define DECODER_H

#include <QObject>
#include <opencv2/opencv.hpp>

class Decoder : public QObject
{
    Q_OBJECT
public:
    Decoder();
public slots:
    cv::Mat decode(QByteArray& data);
};

#endif // DECODER_H
