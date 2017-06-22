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
    //    connect(&w_login, &LoginWindow::logedIn, this, &Controller::startListener);

    connect(MsgQ::instance(), &MsgQ::newmsg, this, &Controller::inputMessage);
    connect(&w_login, &LoginWindow::logedIn, this, &Controller::setNicknameAndIp);
    //provide network interfaces for login
    getAllInterfaces();
    connect(this, SIGNAL(loginInterfaces(QStringList)),
            &w_login, SLOT(setInterfaces(QStringList)));
    emit loginInterfaces(interfaces);
    //broadcast our satus on timeout
    connect(&timer, &QTimer::timeout, this, &Controller::sendToAll);
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

void Controller::configureTray()
{
    _tray.setIcon(QIcon("://icons/ico.ico"));
    QAction * loginAct = new QAction(tr("&Log In"), this);
    QAction * quitAct = new QAction(tr("&Quit"), this);
    connect(loginAct, SIGNAL(triggered(bool)), this, SLOT(userLoginActionHandle()));
    connect(quitAct, SIGNAL(triggered(bool)), this, SLOT(quitActionHandle()));
    menu.addAction(loginAct);
    menu.addAction(quitAct);
    _tray.setContextMenu(&menu);
    _tray.show();
}

void Controller::userLoginActionHandle()
{
    qDebug()<<"userLogin()";
    w_login.show();
}


void Controller::newMsgActionHandle()
{
    QString ip = _ip.left(_ip.lastIndexOf('.')+1)+"255";
    QByteArray nic;
    Sender broadcast(ip,
                     UDP_LISTENER_PORT,
                     TYPE_WHO_IS_THERE,
                     nic,
                     _status);
    broadcast.send();

    w_newMsg.show();
}

void Controller::quitActionHandle()
{
    exit(0);
}

void Controller::enableActionsAfterLogin()
{
    QAction * newmsgAct = new QAction(tr("&New Message"), this);
    QAction * busyStatusAct = new QAction(tr("&I'm busy"), this);
    menu.removeAction(menu.actions().at(0));
    menu.insertAction(menu.actions().at(0), busyStatusAct);
    busyStatusAct->setCheckable(true);
    connect(busyStatusAct, &QAction::toggled, this,  &Controller::setStatus);
    menu.insertAction(menu.actions().at(0), newmsgAct);
    connect(newmsgAct, SIGNAL(triggered(bool)), this, SLOT(newMsgActionHandle()));
}

QString Controller::getIp() const
{
    return _ip;
}

void Controller::setIp(const QString &ip)
{
    _ip = ip;
    startListener();
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
        msg.append(" - " + QString::number(message.data[1]) +  ip);
        w_newMsg.updateCombobox(msg);
    }
    else if (TYPE_NEW_MESSAGE == type)
    {
        QString msg(message.data+1);
        showPopup("nado nick", msg, message.addr);
    }
    else if (TYPE_WHO_IS_THERE == type)
    {
//        Sender s(message.addr.sin_addr.s_addr,
//                 UDP_LISTENER_PORT,
//                 TYPE_STATUS,
//                 _nickname.toLatin1(),
//                 _status);
        QString ip = "172.20.8.130";
        QByteArray data = _nickname.toLatin1();
        Sender s(ip, UDP_LISTENER_PORT, TYPE_STATUS,
                 data, _status);
        s.send();
    }
    delete[] message.data;
}

void Controller::getAllInterfaces()
{
    interfaces.clear();
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

    //TODO
    //call wtis when work is done
    freeifaddrs(i_list);
}

int Controller::sendTo(QString ip, int port, QString buf)
{
    //TODO
    return 0;
}

int Controller::sendToAll()
{
    QString ip = _ip.left(_ip.lastIndexOf('.')+1)+"255";
    QByteArray nic = _nickname.toLatin1();
    Sender broadcast(ip,
                     UDP_LISTENER_PORT,
                     TYPE_STATUS,
                     nic,
                     _status);
    if ( SUCCESS == broadcast.send() )
    {
        return SUCCESS;
    }
    else
    {
        return SENDER_PROC_ERROR;
    }
}

int Controller::startListener()
{

    Listener::instance()->init(_ip, UDP_LISTENER_PORT);
    return  Listener::instance()->start();

    return SUCCESS;
}
