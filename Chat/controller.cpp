#include <pthread.h>
#include <QDebug>

#include "controller.h"
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/wait.h>

Controller *Controller::s_instance = nullptr;

Controller::Controller(QObject *parent) : QObject(parent)
{
    _status = ONLINE;

    connect(MsgQ::instance(), &MsgQ::newmsg, this, &Controller::inputMessage);
    connect(&_w_login, &LoginWindow::logedIn, this, &Controller::setNicknameAndIp);

    getAllInterfaces();

}

QString Controller::getNickname() const
{
    return _nickname;
}

void Controller::setStatus(bool st)
{
    if (true == st)
    {
        _status = BUSY;
    }
    else
    {
        _status = ONLINE;
    }
    qDebug()<<"status - "<<(int)_status;
}

void Controller::showPopup(QString sender, QString msg, sockaddr_in peer)
{
    PopUp *p = new PopUp(sender, msg, peer);
    p->show();
}

void Controller::setNicknameAndIp(const QString &nickname, const QString &ip)
{
    _nickname = nickname;
    _ip = ip;
    enableActionsAfterLogin();
    startListener();
    //    sendToAll();
    //    timer.start(TIMER_INTERVAL);
}

void Controller::configureTrayAndShow()
{
    _tray.setIcon(QIcon("://icons/ico.ico"));
    QAction * loginAct = new QAction(tr("&Log In"), this);
    QAction * quitAct = new QAction(tr("&Quit"), this);
    connect(loginAct, SIGNAL(triggered(bool)), this, SLOT(userLoginActionHandle()));
    connect(quitAct, SIGNAL(triggered(bool)), this, SLOT(quitActionHandle()));
    _menu.addAction(loginAct);
    _menu.addAction(quitAct);
    _tray.setContextMenu(&_menu);
    _tray.show();
}

void Controller::userLoginActionHandle()
{
    qDebug()<<"userLogin()";
    _w_login.show();
}


void Controller::newMsgActionHandle()
{
    QString ip = _ip.left(_ip.lastIndexOf('.')+1)+"255";
    QByteArray nic;
    Sender *broadcast = new Sender(ip,
                                   (char)TYPE_WHO_IS_THERE,
                                   nic,
                                   _status);
    broadcast->start();
    _w_newMsg.clearBox();
    _w_newMsg.show();
}

void Controller::quitActionHandle()
{
    exit(0);
}

void Controller::enableActionsAfterLogin()
{
    QAction * newmsgAct = new QAction(tr("&New Message"), this);
    QAction * busyStatusAct = new QAction(tr("&I'm busy"), this);

    busyStatusAct->setCheckable(true);

    _menu.removeAction(_menu.actions().at(0));
    _menu.insertAction(_menu.actions().at(0), busyStatusAct);
    _menu.insertAction(_menu.actions().at(0), newmsgAct);

    connect(busyStatusAct, &QAction::toggled, this,  &Controller::setStatus);
    connect(newmsgAct, SIGNAL(triggered(bool)), this, SLOT(newMsgActionHandle()));
}

void Controller::inputMessage()
{
    _msg message = MsgQ::instance()->getMsg();
    int type = message.data[0];

    if (TYPE_STATUS == type)
    {
        QString msg(message.data+2);
        char ip[STR_LEN];
        inet_ntop(AF_INET, &message.addr.sin_addr.s_addr, ip, sizeof(ip) );
        QString status = (message.data[1]==BUSY)?"busy":"online";
        msg.append(" - " + status + " " +  ip);
        _w_newMsg.updateCombobox(msg);
    }
    else if (TYPE_NEW_MESSAGE == type)
    {
        QString msg(message.data + message.data[1] + 2);
        char nicname[STR_LEN];
        strncpy(nicname, message.data+2, message.data[1]);
        showPopup(nicname, msg, message.addr);
    }
    else if (TYPE_WHO_IS_THERE == type)
    {
        QByteArray ba =_nickname.toLatin1();

        char cip[255];
        inet_ntop(AF_INET, &message.addr.sin_addr.s_addr, cip, 255 );
        QString ip(cip);
        Sender *s = new Sender(ip,
                               (char)TYPE_STATUS,
                               ba,
                               _status);
        s->start();
    }

    delete[] message.data;
}

void Controller::getAllInterfaces()
{
    QStringList interfaces;
    connect(this, SIGNAL(loginInterfaces(QStringList)),
            &_w_login, SLOT(setInterfaces(QStringList)));
    struct ifaddrs *i_list;

    if (-1 ==  getifaddrs(&i_list))
    {
        //TODO
        //        ERROR("Error: getifaddrs");
        //        return list;
    }

    if (NULL == i_list)
    {
        //TODO
        //        ERROR_MSG("n_Get_Interfaces ERROR");
        //        return list;
    }

    struct sockaddr_in *sa;
    char *addr, buf[STR_LEN];


    for ( int i = 0; i_list; i_list = i_list->ifa_next, i++)
    {
        if (i_list->ifa_addr->sa_family==AF_INET)
        {
            sa = (struct sockaddr_in *) i_list->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            sprintf(buf, "%s %s", i_list->ifa_name, addr);
            interfaces.append( QString(buf) );
        }
    }

    emit loginInterfaces(interfaces);
    freeifaddrs(i_list);
}

int Controller::startListener()
{
    Listener::instance()->init(_ip);
    return  Listener::instance()->start();
}
