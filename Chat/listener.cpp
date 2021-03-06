#include "listener.h"

Listener *Listener::s_instance = nullptr;

Listener::Listener(QObject *parent) : QObject(parent)
{

}

int Listener::init(QString _ip)
{
    ip = _ip;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if ( -1 == sock )
    {
        //TODO
        //ERROR_MSG("Error: listenForPackets - socket() failed.");
        return SOCKET_ERROR;
    }

    // bind the port
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(UDP_LISTENER_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int status = bind(sock,
                      (struct sockaddr *)&addr,
                      sizeof(addr));

    if ( -1 == status )
    {
        //TODO
        //        ERROR_MSG("Error: listenForPackets - bind() failed.");
        close(sock);
        return BIND_ERROR;
    }

    return SUCCESS;
}

int Listener::listen()
{
    qDebug()<<"Listener listen";

    sockaddr_in peer_addr;
    char *buf = new char[BUF_SIZE];
    memset(buf, '\0', BUF_SIZE);

    socklen_t peer_addr_len = sizeof(peer_addr);

    ssize_t result = recvfrom(sock,
                              buf,
                              BUF_SIZE,
                              0,
                              (struct sockaddr *)&peer_addr,
                              &peer_addr_len);

    if ( -1 == result )
    {
        qDebug()<<"Listener recvfrom err";
        //TODO
        //        ERROR_MSG("recvfrom error");
        return ERROR;
    }

    if (inet_addr(ip.toStdString().c_str()) == peer_addr.sin_addr.s_addr)
    {
        qDebug()<<"thread_Listen_Thread--- my pack";
        delete[] buf;
        return MY_MSG_ERROR;
    }
    else
    {
        qDebug()<<"thread_Listen_Thread--- other pack";
        _msg message;
        message.addr = peer_addr;
        message.data = buf;
        MsgQ::instance()->addNewMsg(message);
    }

    return SUCCESS;
}

int Listener::start()
{
    //now create new thread for listen

    int ret;
    pthread_attr_init(&pthread_attr);

    pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);

    ret = pthread_create(&thread,
                         &pthread_attr,
                         &Listener::work,
                         (void*)nullptr);

    if ( 0 != ret )
    {
        //TODO
        //        perror("Error mes: ");
        //        fprintf(stderr,"Error - pthread_create() return error code: %d\n",ret);

        qDebug()<<"Can't start listener thread";
        return PTHREAD_CREATE_ERROR;
    }

    qDebug()<<"Start listener thread";

    return SUCCESS;
}

void *Listener::work(void *)
{
    qDebug()<<"Listener work";
    while (1)
    {
        Listener::instance()->listen();
    }
}

Listener::~Listener()
{
    pthread_attr_destroy(&pthread_attr);
}
