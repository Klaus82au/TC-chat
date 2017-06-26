#include <QApplication>

#include "controller.h"
#include "msgq.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MsgQ::instance();
    Controller::instance()->configureTrayAndShow();

    return a.exec();
}
