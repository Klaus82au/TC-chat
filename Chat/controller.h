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
//#include "sender.h"
#include "loginwindow.h"
#include "popup.h"
#include "messagewindow.h"

class Controller : public QObject
{
    Q_OBJECT
    explicit Controller(QObject *parent = 0);
    Controller ( const Controller & ) = delete;
    Controller ( Controller && ) = delete;
    static Controller *s_instance;

    QString _ip;
    QString _nickname;
    char _status;
    QSystemTrayIcon _tray;
    QMenu _menu;

    LoginWindow _w_login;
    MessageWindow _w_newMsg;

    pthread_mutex_t mutex;

    void getAllInterfaces();

    int startListener();

    void setStatus(bool st);

    void showPopup(QString sender, QString msg, sockaddr_in peer);

public:
    static Controller* instance()
    {
        if (!s_instance)
            s_instance = new Controller();
        return s_instance;
    }

    QString getNickname() const;

signals:
    void loginInterfaces(QStringList list);

private slots:
    void inputMessage();
    void setNicknameAndIp(const QString &nickname, const QString &ip);

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

public slots:
    //sets up and show()'s tray
    void configureTrayAndShow();
};

#endif // CONTROLLER_H
