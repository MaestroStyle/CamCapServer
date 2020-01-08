#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>

#include <opencv2/opencv.hpp>

#include "Decoder.h"

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
    QHostAddress command_address;
    QHostAddress data_address;
    quint16 command_port = 0;
    quint16 data_port = 0;
    bool process_receive = false;
    bool server_running = false;

    quint32 frame_width = 0;
    quint32 frame_height = 0;
    qint32 frame_type = 0;
    quint32 frame_pix_size = 0;
    QByteArray buf_data_frame;
signals:
    void started();
    void stopped();
    void aborted();
    void receiveStarted();
    void receiveStopped();
    void receiveAborted();
    void frameReceived(cv::Mat& frame);
public slots:
    void start(QHostAddress& command_address, quint16 command_port, QHostAddress& data_address, quint16 data_port);
    void receiveCommand();
    void receiveFrame();
    void stop();
};
#endif // RECEIVER_H
