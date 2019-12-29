#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QObject>

#include <QUdpSocket>
#include <QHostAddress>

#include <opencv2/core.hpp>

#define DEBUG_MODE

class Transmitter
{
public:
    Transmitter();

    void setAdress(QHostAddress& address);
    void setPort(quint16 port);
    QHostAddress getAdress();
    quint16 getPort();
private:
    QUdpSocket socket;
    QHostAddress address;
    quint16 port = 0;
    quint64 max_size_datagram = 0;
signals:
    void started();
    void stopped();
public slots:
    void start(QHostAddress& address, quint16 port);
    void stop();
    void transmitFrame(cv::Mat& frame);
};

#endif // TRANSMITTER_H
