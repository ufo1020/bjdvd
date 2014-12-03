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

        if (hex_code < ASCII_CODE) {
            bytes.append(str.setNum(hex_code, 16));
        }
        else {
            uint bytes_sequence = 0;
            while (hex_code) {
                // it is the 1st byte
                if (hex_code >> BITS_PER_UTF8_BYTE == 0) {
                    uint lower_header = UTF_BYTE_HEADER >> bytes_sequence;
                    uint higher_header = ~((1U << (BITS_PER_BYTE - bytes_sequence)) -1U);
                    uint byte = hex_code | lower_header | higher_header;
                    bytes.insert(0, str.setNum(byte, 16));
                }
                else {
                    // the lowest 6 bits
                    uint byte = hex_code | (1U << BITS_PER_UTF8_BYTE - 1U);
                    bytes.insert(0, str.setNum(byte, 16));
                }
                hex_code = hex_code >> BITS_PER_UTF8_BYTE;
                ++bytes_sequence;
            }
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

