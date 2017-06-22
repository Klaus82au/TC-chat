#ifndef POPUP_H
#define POPUP_H

#include <netinet/in.h>
#include <arpa/inet.h>

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QTimer>

#include "messagewindow.h"
#include "global_def.h"

//pop up message widget
class PopUp : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

    void setPopupOpacity(float opacity);
    float getPopupOpacity() const;

    sockaddr_in peer;
    QLabel message;
    QLabel senderNick;
    QPushButton ignoreButton;
    QPushButton replyButton;

    QVBoxLayout topLayout;     // top level layout
    QHBoxLayout buttonsLayout;
    QPropertyAnimation animation;   // Свойство анимации для всплывающего сообщения
    float popupOpacity;     // Свойства полупрозрачности виджета
    QTimer *timer;          // Таймер, по которому виджет будет скрыт


public:
    explicit PopUp(QWidget *parent = 0) = delete;
    PopUp(QString &sender, QString &msg, sockaddr_in peer, QWidget *parent =0);
    ~PopUp();

protected:
    void paintEvent(QPaintEvent *event);    // Фон будет отрисовываться через метод перерисовки

public slots:
    //    void setPopupText(const QString& text); // Установка текста в уведомление
    //    void setPopupSender(const QString &text); // to show sender's nickname
    void show();                            /* Собственный метод показа виджета
                                             * Необходимо для преварительной настройки анимации
                                             * */
    void reply();

private slots:
    void hideAnimation();                   // Слот для запуска анимации скрытия
    void hide();                            /* По окончании анимации, в данном слоте делается проверка,
                                             * виден ли виджет, или его необходимо скрыть
                                             * */
    void ignoreClick();

};

#endif // POPUP_H
