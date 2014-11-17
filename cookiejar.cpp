#include "cookiejar.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QList>
#include <QNetworkCookie>
#include <QVariant>
#include <QSettings>

CookieJar::CookieJar(QObject* parent, const QUrl& url) :
    QNetworkCookieJar(parent),
    m_url(url)
{
    m_has_cookies = load(m_url);
}

CookieJar::~CookieJar()
{
    save(m_url);
}

bool CookieJar::save(const QUrl &url)
{
    QList<QNetworkCookie> cookies = cookiesForUrl(url);

    if (cookies.size() == 0) return false;

    QString directory = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    if (directory.isEmpty()) return false;

    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QSettings cookieSettings(directory + QLatin1String("/cookies.ini"), QSettings::IniFormat);
    cookieSettings.setValue(QLatin1String("cookies"), QVariant::fromValue(cookies));
}

bool CookieJar::load(const QUrl &url)
{
    QString directory = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    if (directory.isEmpty()) return false;

    if (!QFile::exists(directory + QLatin1String("/cookies.ini"))) return false;
    QSettings cookieSettings(directory + QLatin1String("/cookies.ini"), QSettings::IniFormat);
    QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(
                cookieSettings.value(QLatin1String("cookies"), QVariant()));
    if (cookies.size() == 0) return false;
    setCookiesFromUrl(cookies, url);
    return true;
}

bool CookieJar::is_empty() const
{
    return !m_has_cookies;
}
