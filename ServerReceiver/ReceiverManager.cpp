#include "ReceiverManager.h"

ReceiverManager::ReceiverManager(QWidget *parent) : QWidget(parent)
{
    setLayout(&layout);
    layout.addWidget(&command_address_label, 0, 0);
    layout.addWidget(&command_port_label, 0, 1);
    layout.addWidget(&command_address_lineedit, 1, 0);
    layout.addWidget(&command_port_lineedit, 1, 1);
    layout.addWidget(&data_address_label, 2, 0);
    layout.addWidget(&data_port_label, 2, 1);
    layout.addWidget(&data_address_lineedit, 3, 0);
    layout.addWidget(&data_port_lineedit, 3, 1);
    layout.addWidget(&start_button, 4, 0);
    layout.addWidget(&stop_button, 4, 1);

    command_address_label.setText(QString("Command slot address:"));
    command_port_label.setText(QString("Command slot port:"));
    data_address_label.setText(QString("Data slot address:"));
    data_port_label.setText(QString("Data slot port:"));
    start_button.setText(QString("Start"));
    stop_button.setText(QString("Stop"));
    stop_button.setDisabled(true);

    QObject::connect(&start_button, &QPushButton::clicked, this, &ReceiverManager::start);
    QObject::connect(&stop_button, &QPushButton::clicked, this, &ReceiverManager::stop);
}
void ReceiverManager::start(){
//    command_address.setAddress(command_address_lineedit.text());
//    data_address.setAddress(data_address_lineedit.text());
    QHostAddress command_address(command_address_lineedit.text());
    QHostAddress data_address(data_address_lineedit.text());
    quint16 command_port = static_cast<quint16>(command_port_lineedit.text().toUInt());
    quint16 data_port = static_cast<quint16>(data_port_lineedit.text().toUInt());

    if(command_address.isNull() || data_address.isNull() || !command_port || !data_port)
        return;

    start_button.setDisabled(true);
    stop_button.setDisabled(false);
    process = true;

    emit started(command_address, command_port, data_address, data_port);
}
void ReceiverManager::stop(){
    start_button.setDisabled(false);
    stop_button.setDisabled(true);
    process = false;

    emit stopped();
}
void ReceiverManager::error(){
    start_button.setDisabled(false);
    stop_button.setDisabled(true);
    process = false;
}
