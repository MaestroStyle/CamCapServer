#include "Transmitter.h"

Transmitter::Transmitter()
{
    address = QHostAddress::LocalHost;
    port = 5000;
    max_size_datagram = 512;
}

void Transmitter::start(const QHostAddress& address, const quint16 port){
    if(address.isNull()){
#ifdef DEBUG_MODE
        qDebug(QString("Address \"%1\" isn't valid!").arg(address.toString()).toUtf8());
#endif
        return;
    }
    if(port == 0){
#ifdef DEBUG_MODE
        qDebug(QString("Port \"%1\" isn't valid!").arg(port).toUtf8());
#endif
        return;
    }
    server_command_address = address;
    server_command_port = port;

    socket_listener.bind(this->address, this->port);
    socket_sender.writeDatagram(QString("PORT %1 %2").arg(this->address.toString()).arg(this->port).toUtf8(),
                                server_command_address,
                                server_command_port);
    socket_listener.waitForReadyRead(5000);
    QString answer_server = socket_sender.readAll();
    if(answer_server.isEmpty()){
        emit aborted();
        return;
    }
    QStringList server_data_host = answer_server.split(" ");
    server_data_address = QHostAddress(server_data_host[1]);
    server_data_port = server_data_host[2].toUInt();
    if(server_data_host[0] != "PORT" || server_data_address.isNull() || server_data_port == 0){
        emit aborted();
        return;
    }
    process = true;
    emit started();
}
void Transmitter::transmitFrame(cv::Mat& frame){
    socket_sender.writeDatagram("FRAME_BEGIN", server_data_address, server_data_port);
    qint32 count_bytes = frame.total() * frame.channels();
    qint32 cur_offset = 0;
    while (cur_offset + max_size_datagram < count_bytes) {
        socket_sender.writeDatagram((const char*)frame.data + cur_offset, max_size_datagram, server_data_address, server_data_port);
        cur_offset += max_size_datagram;
    }
    socket_sender.writeDatagram((const char*)frame.data + cur_offset, count_bytes - cur_offset, server_data_address, server_data_port);
    socket_sender.writeDatagram("FRAME_END", server_data_address, server_data_port);
}
void Transmitter::stop(){
    process = false;
    socket_sender.writeDatagram("DISCONNECT",
                                server_command_address,
                                server_command_port);
    socket_listener.disconnectFromHost();
    emit stopped();
}
void Transmitter::setPort(quint16 port){
    this->port = port;
}
void Transmitter::setAdress(QHostAddress &address){
    this->address = address;
}
void Transmitter::setMaxSizeDatagram(quint32 max_size_datagram){
    this->max_size_datagram = max_size_datagram;
}
quint16 Transmitter::getPort(){
    return port;
}
QHostAddress Transmitter::getAdress(){
    return address;
}
quint32 Transmitter::getMaxSizeDatagram(){
    return max_size_datagram;
}
