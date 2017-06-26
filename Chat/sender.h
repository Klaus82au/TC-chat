#ifndef SENDER_H
#define SENDER_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <QObject>
#include <QDebug>

#include "global_def.h"

class Sender : public QObject
{
    Q_OBJECT

    int sock;
    sockaddr_in _addr;
    pthread_t thread;
    pthread_attr_t pthread_attr;
    char buf[BUF_SIZE];

    static void* work(void*obj);

public:
    ~Sender();
    Sender(QObject *parent = 0) = delete;
    Sender(QString &_ip, char _type, QByteArray &_data, char _busy = 0, QObject *parent = 0);

    int start();
    int send();

signals:

public slots:
};

#endif // SENDER_H
