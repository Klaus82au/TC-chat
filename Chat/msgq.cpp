#include "msgq.h"

MsgQ *MsgQ::s_instance = nullptr;

MsgQ::MsgQ(QObject *parent) : QObject(parent)
{


}

//MsgQ::~MsgQ()
//{
//    delete msg_list;
//}

MsgQ *MsgQ::instance()
{
    if (!s_instance)
        s_instance = new MsgQ();
    return s_instance;
}

void MsgQ::addNewMsg(_msg msg)
{
    //TODO
    //maybe need lock
    msg_list.push_back(msg);
    emit newmsg();
}

int MsgQ::size()
{
    int size;

    //TODO
    //maybe need lock
    size = msg_list.size();

    return size;
}

_msg MsgQ::getMsg()
{
    //TODO
    //maybe need lock
    _msg s = *(msg_list.begin());
    msg_list.pop_front();

    return s;
}
