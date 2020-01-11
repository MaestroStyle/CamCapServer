#include "Coder.h"

Coder::Coder(){}
void Coder::encode(cv::Mat& frame){
    std::vector<uchar> buf;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG2000_COMPRESSION_X1000);
//    params.push_back(1000);
    cv::imencode("frame.jpeg", frame, buf, params);
    if(buf.empty())
        return;
#ifdef DEBUG_MODE
    qDebug() << "Frame encoded size: " << buf.size();
#endif
    QByteArray data = QByteArray((const char*)buf.data(), static_cast<int>(buf.size()));
    emit encoded(data);
}
