#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <controller.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    explicit Widget(QWidget *parent = 0);
    static Widget *s_instance;

public:
    static Widget* instance()
    {
        if (!s_instance)
            s_instance = new Widget();
        return s_instance;
    }
    ~Widget();


    void setM(QString m);
private slots:

    void on_broadcastbtn_clicked();

    void on_setip_clicked();

    void on_sendbtn_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
