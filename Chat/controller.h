#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSystemTrayIcon>
#include <QMenu>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "global_def.h"
#include "msgq.h"
#include "listener.h"
#include "sender.h"
#include "loginwindow.h"
#include "messagewindow.h"
#include "popup.h"



class Controller : public QObject
{
    Q_OBJECT
    explicit Controller(QObject *parent = 0);
    Controller ( const Controller & ) = delete;
    Controller ( Controller && ) = delete;
    static Controller *s_instance;

    QStringList  interfaces;
    QString _ip;
    QString _nickname;
    char _status;
    QSystemTrayIcon _tray;
    QMenu menu;

    LoginWindow w_login;
    MessageWindow w_newMsg;

public:
    static Controller* instance()
    {
        if (!s_instance)
            s_instance = new Controller();
        return s_instance;
    }

    pthread_mutex_t mutex;

    pthread_t c_listener_thread;
    //TODO mast be destroed in destructor
    pthread_attr_t c_listener_attr;
    QTimer timer;
    void getAllInterfaces();
    //TODO add method to choose interface and set his ip to _ip

    int sendTo(QString ip, int port, QString buf);

    int sendToAll();
    int startListener();

    QString getIp() const;
    void setIp(const QString &ip);

    void setStatus(bool st);

    void showPopup(QString sender, QString msg, sockaddr_in peer);

signals:
    void loginInterfaces(QStringList list);

public slots:

    void inputMessage();
    void setNicknameAndIp(const QString &nickname, const QString &ip);

    //sets up and show()'s tray
    void configureTray();
    //following are menu action handlers
    //enables tray menu item "New message" after user logged in
    void userLoginActionHandle();
    //shows window for message input #### maybe add parameter
    //Contact for instant replying
    void newMsgActionHandle();
    //should terminate the programm
    void quitActionHandle();

    //adds menu acrion new message
    void enableActionsAfterLogin();


};

#endif // CONTROLLER_H
