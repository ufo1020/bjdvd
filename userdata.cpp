#include "userData.h"
#include <iostream>
#include <QDebug>

userData::userData()
{
    QString str("啊");
    to_utf8_bytes(str);
}

userData::~userData()
{
}

QByteArray userData::to_utf8_bytes(QString& string)
{
    for(auto it: string)
    {
        QString str;
        qDebug()<<"unicode "<<str.setNum(it.unicode(), 16);
    }

    return "a";
}
