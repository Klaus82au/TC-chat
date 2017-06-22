#include "popup.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

PopUp::PopUp(QString &sender, QString &msg, sockaddr_in peer, QWidget *parent) : QWidget(parent)
{
    qDebug()<<sender<< msg<<":";
    senderNick.setText(sender);
    message.setText(msg);
    this->peer = peer;

    setWindowFlags(Qt::FramelessWindowHint |        // Отключаем оформление окна
                   Qt::Tool |                       // Отменяем показ в качестве отдельного окна
                   Qt::WindowStaysOnTopHint);       // Устанавливаем поверх всех окон
    setAttribute(Qt::WA_TranslucentBackground);     // Указываем, что фон будет прозрачным
    setAttribute(Qt::WA_ShowWithoutActivating);     // При показе, виджет не получается фокуса автоматически

    animation.setTargetObject(this);                // Устанавливаем целевой объект анимации
    animation.setPropertyName("popupOpacity");      // Устанавливаем анимируемое свойство
    connect(&animation, &QAbstractAnimation::finished, this, &PopUp::hide); /* Подключаем сигнал окончания
                                                                             * анимации к слоты скрытия
                                                                             * */

    // Настройка текста уведомления
    message.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); // Устанавливаем по центру
    // И настраиваем стили
    message.setStyleSheet("QLabel { color : white; "
                          "margin-top: 6px;"
                          "margin-bottom: 6px;"
                          "margin-left: 10px;"
                          "margin-right: 10px; }");
    senderNick.setStyleSheet("color: white");
    // Производим установку текста в размещение, ...
    topLayout.addWidget(&senderNick);
    topLayout.addWidget(&message);
    //configuring the buttons
    replyButton.setText("Reply...");
    ignoreButton.setText("Ignore");

    //adding the buttons
    replyButton.setStyleSheet("background-color: grey; color: white");
    ignoreButton.setStyleSheet("background-color: grey; color: white");
    buttonsLayout.addWidget(&replyButton);
    buttonsLayout.addWidget(&ignoreButton);
    topLayout.addLayout(&buttonsLayout);
    setLayout(&topLayout); // которое помещаем в виджет

    adjustSize();
    // По сигналу таймера будет произведено скрытие уведомления, если оно видимо
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &PopUp::ignoreClick);
    connect(&ignoreButton, &QPushButton::clicked, this, &PopUp::ignoreClick);
    connect(&replyButton, &QPushButton::clicked, this, &PopUp::reply);
    //connect(&replyButton, &QPushButton::clicked, this, &PopUp::deleteLater);

}

PopUp::~PopUp(){
    qDebug()<<"destroying";
    delete timer;
}

void PopUp::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    /* А теперь настраиваем фон уведомления,
     * который является прямоугольником с чёрным фоном
     * */
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // Включаем сглаживание

    // Подготавливаем фон. rect() возвращает внутреннюю геометрию виджета уведомления, по содержимому
    QRect roundedRect;
    roundedRect.setX(rect().x() + 5);
    roundedRect.setY(rect().y() + 5);
    roundedRect.setWidth(rect().width() - 10);
    roundedRect.setHeight(rect().height() - 10);

    // Кисть настраиваем на чёрный цвет в режиме полупрозрачности 180 из 255
    painter.setBrush(QBrush(QColor(0,0,0,180)));
    painter.setPen(Qt::NoPen); // Край уведомления не будет выделен

    // Отрисовываем фон с закруглением краёв в 10px
    painter.drawRoundedRect(roundedRect, 10, 10);
}

void PopUp::show()
{
    setWindowOpacity(0.0);  // Устанавливаем прозрачность в ноль

    animation.setDuration(150);     // Настраиваем длительность анимации
    animation.setStartValue(0.0);   // Стартовое значение будет 0 (полностью прозрачный виджет)
    animation.setEndValue(1.0);     // Конечное - полностью непрозрачный виджет

    setGeometry(QApplication::desktop()->availableGeometry().width() - 36 - width() + QApplication::desktop() -> availableGeometry().x(),
                QApplication::desktop()->availableGeometry().height() - 36 - height() + QApplication::desktop() -> availableGeometry().y(),
                width(),
                height());
    QWidget::show();                // Отображаем виджет, который полностью прозрачен

    animation.start();              // И запускаем анимацию
    timer->start(POPUP_TIME_LIVE);             // А также стартуем таймер, который запустит скрытие уведомления через 3 секунды
}

void PopUp::reply()
{
    //TODO
    //maybe we will need to pass who are we replying to
    MessageWindow * w_newMsg = new MessageWindow();

    char ip[STR_LEN];
    inet_ntop(AF_INET, &peer.sin_addr.s_addr, ip, sizeof(ip) );
    QString sender = QString(senderNick.text()+" "+ip);

    w_newMsg->updateCombobox(sender);
    w_newMsg->show();

    deleteLater();
}

void PopUp::ignoreClick()
{
    hideAnimation();
    deleteLater();
}

void PopUp::hideAnimation()
{
    timer->stop();                  // Останавливаем таймер
    animation.setDuration(1000);    // Настраиваем длительность анимации
    animation.setStartValue(1.0);   // Стартовое значение будет 1 (полностью непрозрачный виджет)
    animation.setEndValue(0.0);     // Конечное - полностью прозрачный виджет
    animation.start();              // И запускаем анимацию
}

void PopUp::hide()
{
    // Если виджет прозрачный, то скрываем его
    if(getPopupOpacity() == 0.0){
        QWidget::hide();
    }
}

void PopUp::setPopupOpacity(float opacity)
{
    popupOpacity = opacity;

    setWindowOpacity(opacity);
}

float PopUp::getPopupOpacity() const
{
    return popupOpacity;
}
