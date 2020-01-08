#ifndef DECODER_H
#define DECODER_H

#include <QObject>
#include <opencv2/opencv.hpp>

class Decoder : public QObject
{
    Q_OBJECT
public:
    Decoder();
signals:
    void decoded(cv::Mat& frame_decoded);
public slots:
    void decode(QByteArray& data);
};

#endif // DECODER_H
