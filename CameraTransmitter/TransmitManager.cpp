#include "TransmitManager.h"

TransmitManager::TransmitManager(QWidget *parent) : QWidget(parent)
{
    server_address_label.setText(QString("Server address:"));
    server_port_label.setText(QString("Server port:"));
    client_address_label.setText(QString("Client address:"));
    client_port_label.setText(QString("Client port:"));
    start_button.setText(QString("Start"));
    stop_button.setText(QString("Stop"));
    stop_button.setDisabled(true);

    server_address_lineedit.setText(hosts.value("server_address", "").toString());
    server_port_lineedit.setText(hosts.value("server_port","").toString());
    client_address_lineedit.setText(hosts.value("client_address","").toString());
    client_port_lineedit.setText(hosts.value("client_port","").toString());

    setLayout(&layout);
    layout.addWidget(&server_address_label, 0, 0);
    layout.addWidget(&server_port_label, 0, 1);
    layout.addWidget(&server_address_lineedit, 1,0);
    layout.addWidget(&server_port_lineedit, 1,1);
    layout.addWidget(&client_address_label, 2, 0);
    layout.addWidget(&client_port_label, 2, 1);
    layout.addWidget(&client_address_lineedit, 3,0);
    layout.addWidget(&client_port_lineedit, 3,1);
    layout.addWidget(&start_button, 4, 0);
    layout.addWidget(&stop_button, 4,1);

    QObject::connect(&start_button, &QPushButton::clicked, this, &TransmitManager::start);
    QObject::connect(&stop_button, &QPushButton::clicked, this, &TransmitManager::stop);
}
TransmitManager::~TransmitManager(){
    hosts.setValue("server_address", server_address_lineedit.text());
    hosts.setValue("server_port", server_port_lineedit.text());
    hosts.setValue("client_address", client_address_lineedit.text());
    hosts.setValue("client_port", client_port_lineedit.text());
}
void TransmitManager::start(){
    QHostAddress server_address(server_address_lineedit.text());
    QHostAddress client_address(client_address_lineedit.text());
    quint16 server_port = static_cast<quint16>(server_port_lineedit.text().toUInt());
    quint16 client_port = static_cast<quint16>(client_port_lineedit.text().toUInt());
    if(server_address.isNull() || client_address.isNull()){
#ifdef DEBUG_MODE
        qDebug(QString("Address \"%1\" isn't valid!").arg(server_address_lineedit.text()).toUtf8());
#endif
        return;
    }
    if(!server_port || !client_port){
#ifdef DEBUG_MODE
        qDebug(QString("Port \"%1\" isn't valid!").arg(server_port_lineedit.text()).toUtf8());
#endif
        return;
    }
    start_button.setDisabled(true);
    stop_button.setDisabled(false);

    process = true;
    emit started(server_address, server_port, client_address, client_port);
}
void TransmitManager::stop(){
    stop_button.setDisabled(true);
    start_button.setDisabled(false);
    process = false;
    emit stopped();
}
bool TransmitManager::isProcess(){
    return process;
}
void TransmitManager::error(){
    process = false;
    stop_button.setDisabled(true);
    start_button.setDisabled(false);
 }
