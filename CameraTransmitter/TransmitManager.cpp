#include "TransmitManager.h"

TransmitManager::TransmitManager(QWidget *parent) : QWidget(parent)
{
    address_label.setText(QString("Adress:"));
    port_label.setText(QString("Port:"));
    start_button.setText(QString("Start"));
    stop_button.setText(QString("Stop"));
    stop_button.setDisabled(true);

    setLayout(&layout);
    layout.addWidget(&address_label, 0, 0);
    layout.addWidget(&port_label, 0, 1);
    layout.addWidget(&address_lineedit, 1,0);
    layout.addWidget(&port_lineedit, 1,1);
    layout.addWidget(&start_button, 2, 0);
    layout.addWidget(&stop_button, 2,1);

    QObject::connect(&start_button, &QPushButton::clicked, this, &TransmitManager::start);
    QObject::connect(&stop_button, &QPushButton::clicked, this, &TransmitManager::stop);
}

void TransmitManager::start(){
    QHostAddress address(address_lineedit.text());
    quint16 port = static_cast<quint16>(port_lineedit.text().toUInt());
    if(address.isNull()){
#ifdef DEBUG_MODE
        qDebug(QString("Address \"%1\" isn't valid!").arg(address_lineedit.text()).toUtf8());
#endif
        return;
    }
    if(port == 0){
#ifdef DEBUG_MODE
        qDebug(QString("Port \"%1\" isn't valid!").arg(port_lineedit.text()).toUtf8());
#endif
        return;
    }
    start_button.setDisabled(true);
    stop_button.setDisabled(false);

    process = true;
    emit started(address, port);
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
