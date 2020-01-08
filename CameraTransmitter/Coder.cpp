#include "Coder.h"

Coder::Coder()
{

}
QByteArray Coder::encode(cv::Mat& frame){
//    frame.convertTo()
    std::vector<uchar> buf;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG2000_COMPRESSION_X1000);
//    params.push_back(1000);
    cv::imencode("frame.jpeg", frame, buf, params);
    if(buf.empty())
        return QByteArray();
    qDebug() << "Frame encoded size: " << buf.size();
    return QByteArray((const char*)buf.data(), static_cast<int>(buf.size()));
}
