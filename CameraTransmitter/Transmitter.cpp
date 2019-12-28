#include "Transmitter.h"

Transmitter::Transmitter()
{
    address_label.setText(QString("Adress:"));
    port_label.setText(QString("Port:"));
    start_button.setText(QString("Start"));
    stop_button.setText(QString("Stop"));

    layout.addWidget(&address_label, 0, 0);
    layout.addWidget(&port_label, 0, 1);
    layout.addWidget(&address_lineedit, 1,0);
    layout.addWidget(&port_lineedit, 1,1);
    layout.addWidget(&start_button, 2, 0);
    layout.addWidget(&stop_button, 2,1);

}

void Transmitter::startTransmit(){
    address.setAddress(address_lineedit.text());
    if(address.isNull()){
#ifdef DEBUG_MODE
        qDebug(QString("Address \"%!\" isn't valid!").arg(address_lineedit.text()).toUtf8());
#endif
        return;
    }

}
