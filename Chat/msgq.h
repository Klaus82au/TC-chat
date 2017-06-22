#ifndef MSGQ_H
#define MSGQ_H

#include <QObject>
#include <QString>
#include <pthread.h>
#include <list>


#include <arpa/inet.h>

typedef void* pointer;

using std::list;

struct _msg{
    sockaddr_in addr;
    char *data;
};

class MsgQ : public QObject
{
    Q_OBJECT
    explicit MsgQ(QObject *parent = 0);
    MsgQ ( const MsgQ & ) = delete;
    MsgQ ( MsgQ && ) = delete;
    static MsgQ *s_instance ;

    pthread_mutex_t mutex;

    list<_msg> msg_list;

public:
    static MsgQ* instance();

    void addNewMsg(_msg _msg);
    int size();
    _msg getMsg();

signals:
    void newmsg();
public slots:
};

#endif // MSGQ_H
