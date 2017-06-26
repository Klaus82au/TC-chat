#ifndef LISTENER_H
#define LISTENER_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <QObject>
#include <QDebug>

#include "msgq.h"
#include "global_def.h"
#include "sender.h"

class Listener : public QObject
{
    Q_OBJECT

    int sock;
    sockaddr_in addr;
    pthread_attr_t pthread_attr;
    pthread_t thread;

    QString ip;

    static Listener* s_instance;
    explicit Listener(QObject *parent = 0);

    int listen();
    static void* work(void*);

public:
    ~Listener();
    static Listener* instance()
    {
        if (!s_instance)
            s_instance = new Listener();
        return s_instance;
    }

    int init(QString _ip);

    int start();

signals:

public slots:
};

#endif // LISTENER_H
