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
    QString m_username;
    QString m_password;
};

#endif // USERDATA_H
