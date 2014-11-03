#include "networkAccess.h"
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QNetworkRequest>
#include <QList>
#include <QVariant>
#include <QNetworkCookieJar>

networkAccess::networkAccess(QObject *parent) : QObject(parent),
    mLoginUrl("http://www.bjdvd.org/signin/"),
    mMainUrl("http://www.bjdvd.org/")
{
    manager.setCookieJar(new QNetworkCookieJar(this));
    connect(&manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));

    loginGet();
}

networkAccess::~networkAccess()
{

}

void networkAccess::loginGet()
{
    QNetworkRequest request(mLoginUrl);
//    TODO User agent format:
//    Mozilla/[version] ([system and browser information]) [platform] ([platform details]) [extensions].
//    For example, Safari on the iPad has used the following: Mozilla/5.0
//    (iPad; U; CPU OS 3_2_1 like Mac OS X; en-us) AppleWebKit/531.21.10 (KHTML, like Gecko)
//    Mobile/7B405 The components of this string are as follows:

//    Mozilla/5.0: Previously used to indicate compatibility with the Mozilla rendering engine
//    (iPad; U; CPU OS 3_2_1 like Mac OS X; en-us): Details of the system in which the browser is running
//    AppleWebKit/531.21.10: The platform the browser uses
//    (KHTML, like Gecko): Browser platform details
//    Mobile/7B405: This is used by the browser to indicate specific enhancements that are available directly in the browser or through third parties.
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.124 Safari/537.36");

    reply = manager.get(request);

}

void networkAccess::loginPostPassword()
{
    //csrfmiddlewaretoken=1955c5b837fe02710ec439c926641549&
    //username=%E5%8D%95%E7%A8%8B%E5%B7%B4%E5%A3%AB&password=3232917&remember_me=on&next=
    QString csrftoken;

    QNetworkRequest request(mMainUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader, 121);

    // set cookies
    QList<QNetworkCookie> cookies = manager.cookieJar()->cookiesForUrl(mLoginUrl);
    for(QNetworkCookie cookie : cookies)
    {
        // get csrftoken value
        if(cookie.name() == QByteArray("csrftoken"))
        {
            csrftoken = cookie.value();
        }
        request.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookie));
        qDebug() << "POST Header COOKIES " << cookie;
    }

    // handle redirect(302 request)
//    QVariant new_location = reply->header(QNetworkRequest::LocationHeader);
//    if (new_location.isValid())
//    {
//        QUrl redirect_url(new_location.toString());
//        if(redirect_url != mMainUrl){
//            qDebug()<<"Redirecting to UNEXPECTED URL";
//            request.setUrl(redirect_url);
//        }
//    }

    //TODO User agent format:
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.124 Safari/537.36");

    if (csrftoken.size() == 0)
    {
        qDebug()<<"csrftoken WRONG!!! Abort...";
        return;
    }
    QByteArray postData;
    postData.append("csrfmiddlewaretoken=");
    postData.append(csrftoken);
    postData.append("&");
    postData.append("username=%E5%8D%95%E7%A8%8B%E5%B7%B4%E5%A3%AB&");
    postData.append("password=3232917&");
    postData.append("remember_me=on&next=");

    reply = manager.post(request, postData);
}

void networkAccess::replyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError){
        qWarning() << "ERROR:" << reply->errorString();
        return;
    }

    mLoginState++;
    qDebug()<<"reply ready read "<<mLoginState;
    switch (mLoginState)
    {
    case STATES::UNKNOWN:
        break;
    case STATES::LOGIN_GET:
        loginGetReadyRead();
        break;
    case STATES::LOGIN_POST:
        loginPostReadyRead();
        break;
    case STATES::REDIRECT:
        mainPageReadyRead();
        break;
    default:
        break;
    }
}

void networkAccess::loginGetReadyRead()
{
    qDebug()<<"slot get Ready Read";
//    qDebug()<<reply->readAll();

    QVariant variantCookies = reply->header(QNetworkRequest::SetCookieHeader);
    QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(variantCookies);
//    qDebug() << "Cookies reply: " << cookies;

    manager.cookieJar()->setCookiesFromUrl(cookies, reply->request().url());
//    qDebug() << "Saved cookies: " << manager.cookieJar()->getAllCookies();
    qDebug() << "COOKIES " << manager.cookieJar()->cookiesForUrl(mLoginUrl);

    loginPostPassword();
}

void networkAccess::loginPostReadyRead()
{
    qDebug()<<"slot post Ready Read";
//    qDebug()<<reply->readAll();

    // set-cookie has sessionid
    QVariant variantCookies = reply->header(QNetworkRequest::SetCookieHeader);
    QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(variantCookies);
    qDebug() << "Cookies reply: " << cookies;

    QVariant variantLocation = reply->header(QNetworkRequest::LocationHeader);
    QUrl redirectURL = qvariant_cast<QUrl>(variantLocation);
    qDebug()<<"location "<<redirectURL;

    redirect(redirectURL);
}

void networkAccess::mainPageReadyRead()
{
    qDebug()<<reply->readAll();
}

void networkAccess::slotError(QNetworkReply::NetworkError e)
{
    qDebug()<<"slotError "<< e;
}

void networkAccess::redirect(const QUrl &url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.124 Safari/537.36");

    QVariant variantCookies;
    variantCookies.setValue(manager.cookieJar()->cookiesForUrl(mLoginUrl));
    request.setHeader(QNetworkRequest::CookieHeader,  variantCookies);
    qDebug() << "POST Header COOKIES " << manager.cookieJar()->cookiesForUrl(mLoginUrl);

    reply = manager.get(request);
}
