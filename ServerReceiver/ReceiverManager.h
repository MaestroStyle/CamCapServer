#ifndef RECEIVERMANAGER_H
#define RECEIVERMANAGER_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHostAddress>

#include <QSettings>

class ReceiverManager : public QWidget
{
    Q_OBJECT
public:
    explicit ReceiverManager(QWidget *parent = nullptr);
    ~ReceiverManager();

    bool isProcess();
private:
    QGridLayout layout;
    QLabel command_address_label;
    QLabel data_address_label;
    QLabel command_port_label;
    QLabel data_port_label;
    QLineEdit command_address_lineedit;
    QLineEdit data_address_lineedit;
    QLineEdit command_port_lineedit;
    QLineEdit data_port_lineedit;
    QPushButton start_button;
    QPushButton stop_button;

    QSettings hosts;

//    QHostAddress command_address;
//    QHostAddress data_address;
//    quint16 command_port = 0;
//    quint16 data_port = 0;
    bool process = false;
signals:
    void started(QHostAddress command_address, quint16 command_port, QHostAddress data_address, quint16 data_port);
    void stopped();
public slots:
    void start();
    void stop();
    void error();
};

#endif // RECEIVERMANAGER_H
