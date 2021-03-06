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
    QByteArray bytes;
    for(auto it: string)
    {
        QString str;
        uint hex_code = it.unicode();

        if (hex_code < BITS_OF_CODE_POINT_ONE_BYTE) {
            bytes.append(str.setNum(hex_code, 16));
        }
        else {
            int bytes_in_sequence = get_bytes_in_sequence(hex_code);
            Q_ASSERT(bytes_in_sequence > 0);

            for(int i = 0; i < bytes_in_sequence - 1 ; i++) {
                // the lowest 6 bits
                uchar byte = (hex_code & ((1U << BITS_PER_UTF8_BYTE) - 1U)) | BYTE_MASK;
                bytes.insert(0, str.setNum(byte, 16));
                hex_code = hex_code >> BITS_PER_UTF8_BYTE;
            }
            // is it the 1st byte

//            uchar lower_header = UTF_BYTE_HEADER >> (bytes_in_sequence - 1U);
//            uchar higher_header = ~((1U << (BITS_PER_BYTE - (bytes_in_sequence - 1U))) -1U);
            uchar byte = hex_code | (BYTE_MASK >> (bytes_in_sequence - 1U));
            bytes.insert(0, str.setNum(byte, 16));

        }
    }

    return "a";
}

uint userData::get_mask(int bytes)
{
    // uft-8 has 1-4 bytes
    Q_ASSERT(bytes >= 1 && bytes <= 4);
    uint bits = BITS_PER_BYTE * bytes - 1;
    constexpr uint limit = BITS_PER_BYTE * sizeof(uint);
    constexpr uint allBits = ~0U;
    uint mask = 0;
    return (bits >= limit) ? allBits : ((1U << bits) - 1U);
}

int userData::get_bytes_in_sequence(uint unicode)
{
    uint bytes = 0;
    if (unicode < BITS_OF_CODE_POINT_ONE_BYTE) {
        bytes = 1;
    }
    else if (unicode < BITS_OF_CODE_POINT_TWO_BYTES) {
        bytes = 2;
    }
    else if (unicode < BITS_OF_CODE_POINT_THREE_BYTES) {
        bytes = 3;
    }
    else if (unicode < BITS_OF_CODE_POINT_FOUR_BYTES) {
        bytes = 4;
    }
    else {
        bytes = -1;
    }

    return bytes;
}

