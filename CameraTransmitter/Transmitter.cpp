#include "Transmitter.h"

Transmitter::Transmitter()
{
    max_size_datagram = 512;
}

void Transmitter::start(QHostAddress& server_address, quint16 server_port, QHostAddress client_address, quint16 client_port){
    if(server_address.isNull() || client_address.isNull()){
        return;
    }
    if(!server_port || !client_port){
        return;
    }
    server_command_address = server_address;
    server_command_port = server_port;
    this->client_address = client_address;
    this->client_port = client_port;

    socket_listener.bind(this->client_address, this->client_port);
    qint32 count_try_connect = 20;
    QString answer_server;
    while(count_try_connect--){
        socket_sender.writeDatagram(QString("PORT %1 %2").arg(this->client_address.toString()).arg(this->client_port).toUtf8(),
                                    server_command_address,
                                    server_command_port);
        if(socket_listener.hasPendingDatagrams()){
            QNetworkDatagram datagram = socket_listener.receiveDatagram();
            answer_server = QString(datagram.data());
            break;
        }
    }
    if(answer_server.isEmpty()){
        emit aborted();
        return;
    }

    QStringList server_data_host = answer_server.split(" ");
    server_data_address = QHostAddress(server_data_host[1]);
    server_data_port = static_cast<quint16>(server_data_host[2].toUInt());
    if(server_data_host[0] != "PORT" || server_data_address.isNull() || !server_data_port){
        emit aborted();
        return;
    }
    process = true;
    emit started();
}
void Transmitter::transmitFrame(cv::Mat& frame){
    Coder coder;
    QByteArray encoded_frame = coder.encode(frame);
    quint32 byte_for_num = 5;
    qint32 cur_offset = 0;
    QByteArray datagram;
    quint32 i = 0;
    socket_sender.writeDatagram(QString("FRAME_BEGIN %1 %2 %3").arg(encoded_frame.size()).arg(byte_for_num).arg(max_size_datagram).toUtf8(),
                                server_data_address,
                                server_data_port);
    while (cur_offset + static_cast<qint32>(max_size_datagram) < encoded_frame.size()) {
        datagram.append(QString::number(i).leftJustified(byte_for_num));
        datagram.append(encoded_frame.data() + cur_offset, max_size_datagram - byte_for_num);
        socket_sender.writeDatagram(datagram, max_size_datagram, server_data_address, server_data_port);
        cur_offset += max_size_datagram - byte_for_num;
        i++;
        qDebug() << datagram.left(byte_for_num);
        qDebug() << datagram.size();
        datagram.clear();
    }
    datagram.append(QString::number(i).leftJustified(byte_for_num));
    datagram.append(encoded_frame.data() + cur_offset, encoded_frame.size() - cur_offset);
    socket_sender.writeDatagram(datagram, encoded_frame.size() - cur_offset, server_data_address, server_data_port);
    socket_sender.writeDatagram("FRAME_END", server_data_address, server_data_port);
}
void Transmitter::stop(){
    process = false;
    socket_sender.writeDatagram("DISCONNECT",
                                server_command_address,
                                server_command_port);
    socket_listener.close();
    emit stopped();
}
void Transmitter::setClientPort(quint16 port){
    this->client_port = port;
}
void Transmitter::setClientAdress(QHostAddress &address){
    this->client_address = address;
}
void Transmitter::setMaxSizeDatagram(quint32 max_size_datagram){
    this->max_size_datagram = max_size_datagram;
}
quint16 Transmitter::getClientPort(){
    return client_port;
}
QHostAddress Transmitter::getClientAdress(){
    return client_address;
}
quint32 Transmitter::getMaxSizeDatagram(){
    return max_size_datagram;
}
