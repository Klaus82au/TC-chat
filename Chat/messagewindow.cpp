#include "messagewindow.h"
#include "ui_messagewindow.h"
#include "controller.h"

MessageWindow::MessageWindow(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::MessageWindow)
{
    ui->setupUi(this);
    ui->peerComboBox->setInsertPolicy(QComboBox::InsertAlphabetically);
    setFixedSize(322, 259);
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

void MessageWindow::clearBox()
{
    ui->peerComboBox->clear();
}

void MessageWindow::closeEvent(QCloseEvent *event )
{
    event->ignore();
    ui->messageEdit->clear();
    hide();

}

void MessageWindow::on_sendButton_clicked()
{
    QByteArray ba = ui->messageEdit->toPlainText().toLatin1();
    //we are sorry
    Sender *s = new Sender(ui->peerComboBox->currentText().split(" ", QString::SkipEmptyParts).last(),
                           TYPE_NEW_MESSAGE,
                           ba);
    s->start();
    ui->messageEdit->clear();
    hide();
}
