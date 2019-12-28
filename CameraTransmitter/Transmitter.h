#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>

#include <QUdpSocket>
#include <QHostAddress>

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

    QGridLayout layout;
    QLabel address_label;
    QLabel port_label;
    QLineEdit address_lineedit;
    QLineEdit port_lineedit;
    QPushButton start_button;
    QPushButton stop_button;
public slots:
    void startTransmit();
    void stopTransmit();
};

#endif // TRANSMITTER_H
