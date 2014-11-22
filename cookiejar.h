#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QList>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QUrl>
#include <QDataStream>

class CookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    CookieJar(QObject* parent, const QUrl& url);
    ~CookieJar();

    bool save(const QUrl& url);
    bool load(const QUrl& url);

    bool is_empty() const;
private:
    QUrl m_url;
    bool m_has_cookies = false;
};

QDataStream &operator<<(QDataStream &out, const QNetworkCookie &cookie);
QDataStream &operator>>(QDataStream &in, QNetworkCookie &cookie);


#endif // COOKIEJAR_H
