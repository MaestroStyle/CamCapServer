#ifndef TRANSMITMANAGER_H
#define TRANSMITMANAGER_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>

#include <QHostAddress>
#include <QSettings>

#define DEBUG_MODE

class TransmitManager : public QWidget
{
    Q_OBJECT
public:
    explicit TransmitManager(QWidget *parent = nullptr);
    ~TransmitManager();

    bool isProcess();
private:
    QGridLayout layout;
    QLabel server_address_label;
    QLabel server_port_label;
    QLineEdit server_address_lineedit;
    QLineEdit server_port_lineedit;
    QLabel client_address_label;
    QLabel client_port_label;
    QLineEdit client_address_lineedit;
    QLineEdit client_port_lineedit;
    QPushButton start_button;
    QPushButton stop_button;

    QSettings hosts;
    bool process = false;
signals:
    void started(QHostAddress& server_address, quint16 server_port, QHostAddress& client_address, quint16 client_port);
    void stopped();
public slots:
    void start();
    void stop();
    void error();
};

#endif // TRANSMITMANAGER_H
