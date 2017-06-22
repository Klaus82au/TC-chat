#include "sender.h"

Sender::~Sender()
{
    close(sock);
}

Sender::Sender(QString &_ip, int _port, char _type, QByteArray &_data, char _busy)
{
//    ip = _ip;
//    port = _port;
//    type = _type;
//    data = _data;
//    busy = _busy;

    memset(buf, '\0', BUF_SIZE);
    strncat(buf, &_type, sizeof(_type));

    switch (_type) {
    case TYPE_STATUS:
        strncat(buf, &_busy, sizeof(_busy));
        strncat(buf, _data.data(), _data.size());
        break;
    case TYPE_NEW_MESSAGE:
        strncat(buf, _data.data(), _data.size());
        break;
    default:
        break;
    }

    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(_ip.toStdString().c_str());
    _addr.sin_port = htons(_port);

}

Sender::Sender(in_addr_t _ip, in_port_t _port, char _type, QByteArray _data, char _busy)
{
    memset(buf, '\0', BUF_SIZE);
    strncat(buf, &_type, sizeof(_type));

    switch (_type) {
    case TYPE_STATUS:
        strncat(buf, &_busy, sizeof(_busy));
        strncat(buf, _data.data(), _data.size());
        break;
    case TYPE_NEW_MESSAGE:
        strncat(buf, _data.data(), _data.size());
        break;
    default:
        break;
    }

    char sm[255];
    inet_ntop(AF_INET, &_ip, sm, 255 );
    int i = ntohs(_port);
    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = _ip;
    _addr.sin_port = _port;
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
        //TODO
        //        printf("%s: cannot send data \n",argv[0]);
        //        close(sock);
        return SENDTO_ERROR;
    }

    return SUCCESS;
}
