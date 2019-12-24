#ifndef SERVER_H
#define SERVER_H

#include <QWidget>

class Server : public QWidget
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();
};
#endif // SERVER_H
