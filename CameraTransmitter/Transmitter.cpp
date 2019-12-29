#include "Transmitter.h"

Transmitter::Transmitter()
{

}

void Transmitter::start(QHostAddress& address, quint16 port){
    if(address.isNull()){
#ifdef DEBUG_MODE
        qDebug(QString("Address \"%!\" isn't valid!").arg(address.toString()).toUtf8());
#endif
        return;
    }
    if(port){
#ifdef DEBUG_MODE
        qDebug(QString("Port \"%!\" isn't valid!").arg(port).toUtf8());
#endif
        return;
    }
    this->address = address;
    this->port = port;
    socket.writeDatagram(QString("%1:%2 ready?").arg(address.toString()).arg(port).toUtf8(), address, port);
    socket.connectToHost(address, port);
//    socket.bind(address, port);
}
void Transmitter::transmitFrame(cv::Mat& frame){
//    if(socket.is)
//    socket.writeDatagram()
}
