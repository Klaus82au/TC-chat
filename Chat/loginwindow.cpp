#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "controller.h"
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->loginButton, SIGNAL(clicked(bool)), this, SLOT(loginSuccess()));
    setFixedSize(162, 132);

}
void LoginWindow::loginSuccess()
{
    qDebug()<<ui->nicknameEdit->text()<<" logged in"<<" "<<
    ui->networkBox->currentText().split(" ", QString::SkipEmptyParts).at(1);
    this->hide();
    //
    emit logedIn(ui->nicknameEdit->text(),
    ui->networkBox->currentText().split(" ", QString::SkipEmptyParts).at(1));
}

void LoginWindow::setInterfaces(QStringList list)
{
    ui->networkBox->addItems(list);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

