#include "widget.h"
#include "ui_widget.h"

Widget *Widget::s_instance = nullptr;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setM(QString m)
{
    ui->output->appendPlainText(m);
}


void Widget::on_broadcastbtn_clicked()
{
    Controller::instance()->sendToAll();
}

void Widget::on_setip_clicked()
{
    Controller::instance()->setIp(ui->ip->text());

}

void Widget::on_sendbtn_clicked()
{
    Controller::instance()->sendTo(ui->ipedit->text(), UDP_LISTENER_PORT, ui->test->text());
}
