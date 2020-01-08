#include "Decoder.h"

Decoder::Decoder()
{

}
cv::Mat Decoder::decode(QByteArray& data){
    if(data.isEmpty())
        return cv::Mat();
    std::vector<char> buf(data.begin(), data.end());
    cv::Mat frame = cv::imdecode(buf, cv::IMREAD_COLOR);
    return frame;
}
