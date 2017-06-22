#include "messagewindow.h"
#include "ui_messagewindow.h"
#include "controller.h"

MessageWindow::MessageWindow(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::MessageWindow)
{
    ui->setupUi(this);
    ui->peerComboBox->setInsertPolicy(QComboBox::InsertAlphabetically);
//    connect(this, &MessageWindow::rejected, this, &MessageWindow::deleteLater);
    //    connect()
}

MessageWindow::~MessageWindow()
{
    qDebug()<<"destructor of newMsgWindows";
    delete ui;
}

void MessageWindow::updateCombobox(QString str)
{
    ui->peerComboBox->addItem(str);
}

void MessageWindow::on_sendButton_clicked()
{
    QByteArray ba = ui->messageEdit->toPlainText().toLatin1();
    //we are sorry
    Sender s(ui->peerComboBox->currentText().split(" ", QString::SkipEmptyParts).last(),
             UDP_LISTENER_PORT,
             TYPE_NEW_MESSAGE,
             ba);
    s.send();
}
