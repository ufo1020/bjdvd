#ifndef USERDATA_H
#define USERDATA_H
#include <QString>
#include <QByteArray>

class userData
{
public:
    userData();
    QByteArray to_utf8_bytes(QString& string){
            QString str = QString::fromUtf8("");
            for (QString::iterator it = str.begin(); it != str.end(); ++it)
            {
                std::cout<< std::hex << it->unicode()<< std::endl;
            }
    }

    void set_username(QString&);
    void set_password(QString&);
private:
    QString m_username;
    QString m_password;
};

#endif // USERDATA_H
