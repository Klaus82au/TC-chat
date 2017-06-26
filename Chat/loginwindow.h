#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
//dialog for login
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

public slots:
    void loginSuccess();
signals:
    void logedIn(QString leNick, QString leIp);
private slots:
    void setInterfaces(QStringList list);
private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
