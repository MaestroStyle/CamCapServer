#include "Transmitter.h"

Transmitter::Transmitter()
{
    max_size_datagram = 500;
    count_try_transmit = 5;
    count_msec_for_ready_read = 1000;
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
#ifdef DEBUG_MODE
    qDebug() << QString("listener: %1:%2 - opened").arg(client_address.toString()).arg(client_port).toUtf8();
#endif
    socket_listener.bind(this->client_address, this->client_port);
    quint32 count_try_connect = count_try_transmit;
    QString answer_server;
    while(count_try_connect--){
        socket_sender.writeDatagram(QString("PORT %1 %2").arg(this->client_address.toString()).arg(this->client_port).toUtf8(),
                                    server_command_address,
                                    server_command_port);
        if(socket_listener.waitForReadyRead(count_msec_for_ready_read)){
            QNetworkDatagram datagram = socket_listener.receiveDatagram();
            answer_server = QString(datagram.data());
            if(answer_server.isEmpty()){
                abort();
                return;
            }
#ifdef DEBUG_MODE
        qDebug() << answer_server;
#endif
            QStringList server_data_host = answer_server.split(" ");
            if(server_data_host.isEmpty() || server_data_host.count() != 3){
                abort();
                return;
            }
            server_data_address = QHostAddress(server_data_host[1]);
            server_data_port = static_cast<quint16>(server_data_host[2].toUInt());
            if(server_data_host[0] != "PORT" || server_data_address.isNull() || !server_data_port){
                abort();
                return;
            }
            count_try_connect = count_try_transmit;

            while(count_try_connect--){
                socket_sender.writeDatagram(QString("OK").toUtf8(),
                                            server_command_address,
                                            server_command_port);
                if(socket_listener.waitForReadyRead(count_msec_for_ready_read)){
                    QNetworkDatagram answer_datagram = socket_listener.receiveDatagram();
#ifdef DEBUG_MODE
        qDebug() << answer_datagram.data();
#endif
                    if(answer_datagram.data() == "OK")
                        break;
                }
            }
            process = true;
            is_ready_transmit = true;
            id_cur_datagram = 0;
            emit started();
            return;
        }
    }
    abort();
}
void Transmitter::transmitFrame(QByteArray frame){
    if(!process)
        return;
    is_ready_transmit = false;
    QByteArray header_frame = "FRAME_BEGIN";
    transmitDatagram(header_frame, count_try_transmit);

    qint32 cur_offset = 0;
    QByteArray datagram;
    while (process && cur_offset + static_cast<qint32>(max_size_datagram) < frame.size()) {
        datagram = QByteArray(frame.data() + cur_offset, max_size_datagram);
        transmitDatagram(datagram, count_try_transmit);
        cur_offset += max_size_datagram;
    }
    datagram = QByteArray(frame.data() + cur_offset, frame.size() - cur_offset);
    transmitDatagram(datagram, count_try_transmit);

    QByteArray footer_frame = "FRAME_END";
    transmitDatagram(footer_frame, count_try_transmit);
    is_ready_transmit = true;
}
bool Transmitter::transmitDatagram(QByteArray& datagram, quint32 count_try){
    if(!process)
        return false;
    QByteArray package;
    package.append(QString::number(id_cur_datagram).append(" "));
    package.append(datagram);
    while(count_try--){
        socket_sender.writeDatagram(package, server_data_address, server_data_port);
        socket_sender.writeDatagram(QString("GOT %1 %2").arg(id_cur_datagram).arg(datagram.size()).toUtf8(), server_command_address, server_command_port);
        if(socket_listener.state() != QAbstractSocket::SocketState::BoundState)
            return false;
        if(socket_listener.waitForReadyRead(count_msec_for_ready_read)){
            QNetworkDatagram answer_datagram = socket_listener.receiveDatagram();
            QString answer_server = QString(answer_datagram.data());
#ifdef DEBUG_MODE
        qDebug() << answer_server;
#endif
            if(answer_datagram.data() == "OK"){
                id_cur_datagram++;
                return true;
            }
        }
    }
    id_cur_datagram++;
    return false;
}
void Transmitter::stop(){
    if(!process)
        return;
    qint32 count_try_connect = count_try_transmit;
    QString answer_server;
    while(count_try_connect--){
        socket_sender.writeDatagram("DISCONNECT",
                                    server_command_address,
                                    server_command_port);
        if(socket_listener.state() != QAbstractSocket::SocketState::BoundState){
            id_cur_datagram = 0;
            is_ready_transmit = false;
            process = false;
            emit stopped();
            return;
        }
        if(socket_listener.waitForReadyRead(count_msec_for_ready_read)){
            QNetworkDatagram datagram = socket_listener.receiveDatagram();
            answer_server = QString(datagram.data());
#ifdef DEBUG_MODE
            qDebug() << answer_server;
#endif
            if(answer_server == "DICSKONNECTED"){
                break;
            }
        }
    }
    process = false;
    is_ready_transmit = false;
    id_cur_datagram = 0;
    socket_listener.close();
#ifdef DEBUG_MODE
    qDebug() << QString("listener: %1:%2 - close").arg(client_address.toString()).arg(client_port).toUtf8();
#endif
    emit stopped();
}
void Transmitter::abort(){
    if(socket_listener.state() != QAbstractSocket::SocketState::BoundState)
        socket_listener.close();
#ifdef DEBUG_MODE
    qDebug() << QString("listener: %1:%2 - close").arg(client_address.toString()).arg(client_port).toUtf8();
#endif
    is_ready_transmit = false;
    process = false;
    emit aborted();
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
bool Transmitter::isReadyTransmit(){
    return is_ready_transmit;
}
