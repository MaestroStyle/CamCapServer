#include "Receiver.h"

Receiver::Receiver()
{
    QObject::connect(&command_socket, &QUdpSocket::readyRead, this, &Receiver::receiveCommand);
    QObject::connect(&data_socket, &QUdpSocket::readyRead, this, &Receiver::receiveFrame);
}
bool Receiver::isProcessReceive(){
    return process_receive;
}
bool Receiver::isServerRunning(){
    return server_running;
}
void Receiver::start(QHostAddress& command_address, quint16 command_port, QHostAddress& data_address, quint16 data_port){
    if(command_address.isNull() || data_address.isNull()){
        emit receiveAborted();
        return;
    }
    this->command_address = command_address;
    this->data_address = data_address;
    this->command_port = command_port;
    this->data_port = data_port;

    server_running = true;
    emit started();
    command_socket.bind(command_address, command_port);
}
void Receiver::receiveCommand(){
    QString command;
    if(command_socket.hasPendingDatagrams()){
            QNetworkDatagram datagram = command_socket.receiveDatagram();
            command = QString(datagram.data());
    }
    if(command.isEmpty())
        return;

    QStringList args_command = command.split(" ");
    if(args_command.isEmpty())
        return;

    if(args_command[0] == "PORT"){
        QHostAddress address(args_command[1]);
        quint16 port = static_cast<quint16>(args_command[2].toUInt());
        if(address.isNull() || !port)
            return;
        if(isProcessReceive()){
            command_socket.writeDatagram(QString("CANCELED").toUtf8(), address, port);
            return;
        }
        data_socket.bind(data_address, data_port);
        process_receive = true;
        emit receiveStarted();
        command_socket.writeDatagram(QString("PORT %1 %2").arg(data_address.toString()).arg(data_port).toUtf8(), address, port);
        return;
    }
    if(args_command[0] == "DISCONNECT"){
        process_receive = false;
        data_socket.close();
        buf_data_frame.clear();
        frame_type = 0;
        frame_width = 0;
        frame_height = 0;
        emit receiveStopped();
        return;
    }
}
void Receiver::receiveFrame(){
    QString data;
    if(data_socket.hasPendingDatagrams()){
        QNetworkDatagram datagram = data_socket.receiveDatagram();
        data = QString(datagram.data());
    }
    if(data.isEmpty())
        return;
    if(data.left(11) == "FRAME_BEGIN"){
        buf_data_frame.clear();
        QStringList args_frame = data.split(" ");
        if(args_frame.isEmpty())
            return;
        frame_width = args_frame[1].toUInt();
        frame_height = args_frame[2].toUInt();
        frame_type = args_frame[3].toInt();
        frame_pix_size = static_cast<quint32>(args_frame[4].toInt());
        if(!frame_width || !frame_height || !frame_pix_size)
            return;
        return;
    }
    if(data.left(9) == "FRAME_END"){
//        qint32 num_byte_missing = frame_width * frame_height * frame_pix_size - buf_data_frame.count();
//        if(num_byte_missing > 0){
//            buf_data_frame.append(num_byte_missing, '0');
//        }
//        cv::Mat frame(static_cast<int>(frame_height), static_cast<int>(frame_width), frame_type, buf_data_frame.data());
        Decoder decoder;
        cv::Mat frame = decoder.decode(buf_data_frame);
        emit frameReceived(frame);
        return;
    }
    buf_data_frame.append(data);
}
void Receiver::stop(){
    if(isProcessReceive()){
        process_receive = false;
        data_socket.close();
        buf_data_frame.clear();
        frame_type = 0;
        frame_width = 0;
        frame_height = 0;
        emit receiveStopped();
    }
    server_running = false;
    command_socket.close();
    emit stopped();
}
