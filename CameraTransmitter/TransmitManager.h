#ifndef TRANSMITMANAGER_H
#define TRANSMITMANAGER_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>

#include <QHostAddress>

#define DEBUG_MODE

class TransmitManager : public QWidget
{
    Q_OBJECT
public:
    explicit TransmitManager(QWidget *parent = nullptr);

    bool isProcess();
private:
    QGridLayout layout;
    QLabel address_label;
    QLabel port_label;
    QLineEdit address_lineedit;
    QLineEdit port_lineedit;
    QPushButton start_button;
    QPushButton stop_button;

    bool process = false;
signals:
    void started(QHostAddress& address, quint16 port);
    void stopped();
public slots:
    void start();
    void stop();
    void error();
};

#endif // TRANSMITMANAGER_H
