#ifndef SENDER_H
#define SENDER_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <QObject>
#include "global_def.h"

class Sender : public QObject
{
    Q_OBJECT

    int sock;
    sockaddr_in _addr;

    char buf[BUF_SIZE];
    //    QByteArray data;
    //    QString ip;

    //    int port;
    //    int sec;
    //    char type;
    //    char busy;

public:
    ~Sender();
    Sender(QObject *parent = 0) = delete;
    Sender(QString &_ip, int _port, char _type, QByteArray &_data, char _busy = 0);
    Sender(in_addr_t _ip, in_port_t _port, char _type, QByteArray _data, char _busy = 0);
    int send();

signals:

public slots:
};

#endif // SENDER_H
