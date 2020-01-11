#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>

#include <opencv2/opencv.hpp>

//#define DEBUG_MODE

class Receiver : public QObject
{
    Q_OBJECT
public:
    Receiver();

    bool isProcessReceive();
    bool isServerRunning();
private:
    QUdpSocket command_socket;
    QUdpSocket data_socket;
    QUdpSocket socket_sender;
    QHostAddress command_address;
    QHostAddress data_address;
    QHostAddress client_address;
    quint16 command_port = 0;
    quint16 data_port = 0;
    quint16 client_port = 0;
    bool process_receive = false;
    bool server_running = false;

    quint32 count_byte = 0;
    quint32 byte_for_num = 0;
    quint32 prev_num_datagram = 0;
    quint32 max_size_datagram = 0;
    QByteArray buf_data_frame;
    QByteArray buf_datagram;
    quint64 id_cur_datagram = 0;
    quint32 count_try_transmit = 0;
    qint32 count_msec_for_ready_read = 0;
signals:
    void started();
    void stopped();
    void aborted();
    void receiveStarted();
    void receiveStopped();
    void receiveAborted();
    void frameReceived(QByteArray data);
public slots:
    void start(QHostAddress command_address, quint16 command_port, QHostAddress data_address, quint16 data_port);
    void receiveCommand();
    void receiveFrame();
    void stop();
};
#endif // RECEIVER_H
