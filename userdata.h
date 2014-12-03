#ifndef USERDATA_H
#define USERDATA_H
#include <QString>
#include <QByteArray>

class userData
{
public:
    userData();
    ~userData();
    QByteArray to_utf8_bytes(QString& string);

    void set_username(QString& username)
    {
        m_username = username;
    }

    void set_password(QString& password)
    {
        m_password = password;
    }

private:
    static const uint BITS_PER_BYTE = 8U;
    static const uint BITS_PER_UTF8_BYTE = 6U;

    static const uint UTF_BYTE_HEADER = 1U << 7U;

    // it must be signed
    static const char BYTE_MASK = 0x80;

    // UTF-8 1 byte for U+000 - U+007F (7 bits)
    static const uint BITS_OF_CODE_POINT_ONE_BYTE = 1U << 7U;
    // UTF-8 2 bytes for U+0080 - U+07FF (11 bits)
    static const uint BITS_OF_CODE_POINT_TWO_BYTES = 1U << 11U;
    // UTF-8 3 bytes for U+0800 - U+FFFF (16 bits)
    static const uint BITS_OF_CODE_POINT_THREE_BYTES = 1U << 16U;
    // UTF-8 4 bytes for U+10000 - U+1FFFFF (21 bits)
    static const uint BITS_OF_CODE_POINT_FOUR_BYTES = 1U << 21U;

    // 1 byte mask: 7F, 2 bytes mask: 7FF, 3 bytes mask:FFFF,
    // 4 bytes mask: 1FFFFF
    uint get_mask(int bytes);

    //
    uint get_bytes(uint unicode);

    int get_bytes_in_sequence(uint unicode);

    QString m_username;
    QString m_password;
};

#endif // USERDATA_H
