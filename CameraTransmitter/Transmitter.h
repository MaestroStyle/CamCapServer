#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QObject>

#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>

#include <opencv2/core.hpp>
#include <Coder.h>

//#define DEBUG_MODE

class Transmitter : public QObject
{
    Q_OBJECT
public:
    Transmitter();

    void setClientAdress(QHostAddress& address);
    void setClientPort(quint16 port);
    void setMaxSizeDatagram(quint32 max_size_datagram);
    QHostAddress getClientAdress();
    quint16 getClientPort();
    quint32 getMaxSizeDatagram();
    bool isReadyTransmit();
private:
    QUdpSocket socket_sender;
    QUdpSocket socket_listener;
    QHostAddress server_command_address;
    QHostAddress server_data_address;
    QHostAddress client_address;
    quint16 server_command_port = 0;
    quint16 server_data_port = 0;
    quint16 client_port = 0;
    quint32 max_size_datagram = 0;
    bool process = false;
    bool is_ready_transmit = false;
    quint64 id_cur_datagram = 0;
    quint32 count_try_transmit = 0;
    qint32 count_msec_for_ready_read = 0;
signals:
    void started();
    void stopped();
    void aborted();
public slots:
    void start(QHostAddress& server_address, quint16 server_port, QHostAddress client_address, quint16 client_port);
    void stop();
    void transmitFrame(QByteArray frame);
    bool transmitDatagram(QByteArray& datagram, quint32 count_try = 3);
    void abort();
};

#endif // TRANSMITTER_H
