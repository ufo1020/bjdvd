#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QNetworkCookieJar>
#include <QUrl>

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

#endif // COOKIEJAR_H
