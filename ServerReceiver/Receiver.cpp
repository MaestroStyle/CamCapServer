#include "Receiver.h"

Receiver::Receiver()
{
    count_try_transmit = 5;
    count_msec_for_ready_read = 1000;

    QObject::connect(&command_socket, &QUdpSocket::readyRead, this, &Receiver::receiveCommand, Qt::DirectConnection);
    QObject::connect(&data_socket, &QUdpSocket::readyRead, this, &Receiver::receiveFrame, Qt::DirectConnection);
}
bool Receiver::isProcessReceive(){
    return process_receive;
}
bool Receiver::isServerRunning(){
    return server_running;
}
void Receiver::start(QHostAddress command_address, quint16 command_port, QHostAddress data_address, quint16 data_port){
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
#ifdef DEBUG_MODE
    qDebug() << QString("command: %1:%2 - opened").arg(command_address.toString()).arg(command_port).toUtf8();
#endif
}
void Receiver::receiveCommand(){
    QString command;
    if(command_socket.hasPendingDatagrams()){
            QNetworkDatagram datagram = command_socket.receiveDatagram();
            command = QString(datagram.data());
    }
    if(command.isEmpty())
        return;
#ifdef DEBUG_MODE
    qDebug() << command;
#endif
    QStringList args_command = command.split(" ");
    if(args_command.isEmpty())
        return;
    if(args_command[0] == "GOT"){
        quint64 id_got_datagram = args_command[1].toULongLong();
        quint32 size_datagram = args_command[2].toUInt();
        if(id_got_datagram != id_cur_datagram || size_datagram != static_cast<quint32>(buf_datagram.size())){
            socket_sender.writeDatagram("REPEAT", client_address, client_port);
        } else{
            socket_sender.writeDatagram("OK", client_address, client_port);
        }
        return;
    }
    if(args_command[0] == "PORT"){
        client_address = QHostAddress(args_command[1]);
        client_port = static_cast<quint16>(args_command[2].toUInt());
        if(client_address.isNull() || !client_port)
            return;
        if(isProcessReceive()){
            socket_sender.writeDatagram(QString("CANCELED").toUtf8(), client_address, client_port);
            return;
        }
        data_socket.bind(data_address, data_port);
#ifdef DEBUG_MODE
    qDebug() << QString("data: %1:%2 - opened").arg(data_address.toString()).arg(data_port).toUtf8();
#endif
        qint32 count_try_connect = count_try_transmit;
        while (count_try_connect--) {
            socket_sender.writeDatagram(QString("PORT %1 %2").arg(data_address.toString()).arg(data_port).toUtf8(), client_address, client_port);
            if(command_socket.state() != QAbstractSocket::SocketState::BoundState){
                process_receive = false;
#ifdef DEBUG_MODE
    qDebug() << QString("data: %1:%2 - close").arg(data_address.toString()).arg(data_port).toUtf8();
#endif
                data_socket.close();
                return;
            }
            if(command_socket.waitForReadyRead(count_msec_for_ready_read)){
                QNetworkDatagram answer_datagram = command_socket.receiveDatagram();
                QString answer_client = QString(answer_datagram.data());
#ifdef DEBUG_MODE
    qDebug() << answer_client;
#endif
                if(answer_client == "OK"){
                    socket_sender.writeDatagram(QString("OK").toUtf8(), client_address, client_port);
                    process_receive = true;
                    buf_datagram.clear();
                    id_cur_datagram = 0;
                    emit receiveStarted();
                    break;
                }
            }
        }
        return;
    }
    if(args_command[0] == "DISCONNECT"){
        process_receive = false;
        data_socket.close();
#ifdef DEBUG_MODE
    qDebug() << QString("data: %1:%2 - close").arg(data_address.toString()).arg(data_port).toUtf8();
#endif
        command_socket.close();
#ifdef DEBUG_MODE
    qDebug() << QString("command: %1:%2 - close").arg(command_address.toString()).arg(command_port).toUtf8();
#endif
        command_socket.bind(command_address, command_port);
#ifdef DEBUG_MODE
    qDebug() << QString("command: %1:%2 - opened").arg(command_address.toString()).arg(command_port).toUtf8();
#endif
        buf_data_frame.clear();
        buf_datagram.clear();
        id_cur_datagram = 0;
        count_byte = 0;
        socket_sender.writeDatagram("DICSKONNECTED", client_address, client_port);
        emit receiveStopped();
        return;
    }
}
void Receiver::receiveFrame(){
    if(!isProcessReceive())
        return;
    if(data_socket.hasPendingDatagrams()){
        QNetworkDatagram datagram = data_socket.receiveDatagram();
        buf_datagram = datagram.data();
    }
    if(buf_datagram.isEmpty())
        return;

    qint32 pos_space = buf_datagram.indexOf(' ');
    if(pos_space == -1)
        return;
    id_cur_datagram = buf_datagram.left(pos_space).toULongLong();
    buf_datagram.remove(0, pos_space + 1);
#ifdef DEBUG_MODE
    qDebug() << "datagram: id: " << id_cur_datagram << "size: " << buf_datagram.size();
    qDebug() << buf_datagram.left(15) << "...";
#endif
    if(buf_datagram == "FRAME_BEGIN"){
        buf_data_frame.clear();
        return;
    }

    if(buf_datagram == "FRAME_END"){
        emit frameReceived(buf_data_frame);
        return;
    }

    buf_data_frame.append(buf_datagram);
}
void Receiver::stop(){
    if(isProcessReceive()){
        process_receive = false;
        data_socket.close();
#ifdef DEBUG_MODE
    qDebug() << QString("data: %1:%2 - close").arg(data_address.toString()).arg(data_port).toUtf8();
#endif
        buf_data_frame.clear();
        buf_datagram.clear();
        count_byte = 0;
        id_cur_datagram = 0;
        emit receiveStopped();
    }
    server_running = false;
    command_socket.close();
#ifdef DEBUG_MODE
    qDebug() << QString("command: %1:%2 - close").arg(command_address.toString()).arg(command_port).toUtf8();
#endif
    emit stopped();
}
