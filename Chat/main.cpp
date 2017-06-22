#include <QApplication>

#include "controller.h"
#include "msgq.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MsgQ::instance();
//    Widget::instance()->show();
    Controller::instance()->configureTray();

    return a.exec();
}
