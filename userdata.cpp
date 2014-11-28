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

uint userData::get_mask(int bytes)
{
    // uft-8 has 1-4 bytes
    Q_ASSERT(bytes >= 1 && bytes <= 4);
    uint bits = BITS_PER_BYTE * bytes;
    constexpr uint limit = BITS_PER_BYTE * sizeof(uint);
    constexpr uint allBits = ~0U;
    uint mask = 0;
    return (bits >= limit) ? allBits : ((1U << bits) - 1U);
}

