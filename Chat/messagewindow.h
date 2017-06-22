#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#include <QDialog>

#include <sender.h>
//input message window
namespace Ui {
class MessageWindow;
}

class MessageWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MessageWindow(QDialog *parent = 0);
    ~MessageWindow();

    //updates peers in combobox
    void updateCombobox(QString str);

private slots:
    void on_sendButton_clicked();

private:
    Ui::MessageWindow *ui;
};

#endif // MESSAGEWINDOW_H
