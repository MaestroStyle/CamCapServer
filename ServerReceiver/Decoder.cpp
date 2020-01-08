#include "Decoder.h"

Decoder::Decoder()
{

}
void Decoder::decode(QByteArray& data){
    if(data.isEmpty())
        return;
    std::vector<char> buf(data.begin(), data.end());
    cv::Mat frame = cv::imdecode(buf, cv::IMREAD_COLOR);
    emit decoded(frame);
}
