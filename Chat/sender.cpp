#include "sender.h"
#include "controller.h"

void *Sender::work(void * obj)
{
    ((Sender*)obj)->send();
    return nullptr;
}

Sender::~Sender()
{
    qDebug()<<"sender destroy";
    pthread_attr_destroy(&pthread_attr);
    close(sock);
}

Sender::Sender(QString &_ip, char _type, QByteArray &_data, char _busy, QObject *parent)
{
    memset(buf, '\0', BUF_SIZE);
    strncat(buf, &_type, sizeof(_type));

    if (TYPE_STATUS == _type)
    {
        strncat(buf, &_busy, sizeof(_busy));
        strncat(buf, _data.data(), _data.size());
    }
    else if (TYPE_NEW_MESSAGE == _type)
    {
        QByteArray nickname = Controller::instance()->getNickname().toLatin1();
        char size = nickname.length();
        strncat(buf, &size, sizeof(size));
        strncat(buf, nickname.data(), nickname.size());
        strncat(buf, _data, _data.size());
    }
    else if (TYPE_WHO_IS_THERE == _type)
    {
    }

    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(_ip.toStdString().c_str());
    _addr.sin_port = htons(UDP_LISTENER_PORT);

}

int Sender::start()
{
    int ret;
    pthread_attr_init(&pthread_attr);

    pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);

    ret = pthread_create(&thread,
                         &pthread_attr,
                         &Sender::work,
                         this);

    if ( 0 != ret )
    {
        //TODO
        //        perror("Error mes: ");
        //        fprintf(stderr,"Error - pthread_create() return error code: %d\n",ret);

        qDebug()<<"Can't start Sender thread";
        return PTHREAD_CREATE_ERROR;
    }

    qDebug()<<"Start Sender thread";

    return SUCCESS;
}

int Sender::send()
{
    int rc;
    sock = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
    if(sock<0)
    {
        //TODO
        //printf("%s: cannot open socket \n",argv[0]);
        return SOCKET_ERROR;
    }

    rc = setsockopt(sock,
                    SOL_SOCKET,
                    SO_BROADCAST,
                    &_addr,
                    sizeof(_addr));
    if ( rc < 0)
    {
        //TODO
        //        printf("setsockopt (SO_BROADCAST)");
        return SETSOCKOPT_ERROR;
    }

    /* send data */
    rc = sendto(sock,
                buf,
                strlen(buf)+1,
                0,
                (struct sockaddr *) &_addr,
                sizeof(_addr));

    if(rc<0)
    {
        qDebug()<<"errno --- "<<errno;
        qDebug()<<"errmsg --- "<<strerror(errno);
        //TODO
        //        printf("%s: cannot send data \n",argv[0]);
        //        close(sock);
        return SENDTO_ERROR;
    }
    deleteLater();
    return SUCCESS;
}
