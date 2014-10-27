#include "networkAccess.h"
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QNetworkRequest>
#include <QList>
#include <QVariant>
#include <QNetworkCookieJar>

networkAccess::networkAccess(QObject *parent) : QObject(parent),
    mLoginUrl(LOGIN_URL),
    mMainUrl(MAIN_URL)
{
    manager.setCookieJar(new QNetworkCookieJar(this));
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    QObject::connect(reply, SIGNAL(readyRead()), this, SLOT(loginGetReadyRead()));
    QObject::connect(reply, SIGNAL(readyRead()), this, SLOT(loginPostReadyRead()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));

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
    QByteArray postData;
    postData.append("csrfmiddlewaretoken=1955c5b837fe02710ec439c926641549&");
    postData.append("username=%E5%8D%95%E7%A8%8B%E5%B7%B4%E5%A3%AB&");
    postData.append("password=3232917&");
    postData.append("remember_me=on&next=");
    QNetworkRequest request(mLoginUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader, 121);

//    QList<QNetworkCookie> cookies;
//    cookies.append(QNetworkCookie("csrftoken", "1955c5b837fe02710ec439c926641549"));
//    cookies.append(QNetworkCookie("sessionid", "6b90405022f9d70127700b82e2f1fc6b"));

    //set cookies
//    QList<QNetworkCookie> cookies = manager.cookieJar()->cookiesForUrl(QUrl(LOGIN_URL));
    QVariant variantCookies;
    variantCookies.setValue(manager.cookieJar()->cookiesForUrl(mLoginUrl));
//    qDebug<<params;
    request.setHeader(QNetworkRequest::CookieHeader,  variantCookies);
    qDebug() << "POST Header COOKIES " << manager.cookieJar()->cookiesForUrl(mLoginUrl);

    //TODO User agent format:
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.124 Safari/537.36");

    reply = manager.post(request, postData);
}

void networkAccess::replyFinished(QNetworkReply* reply)
{
    qDebug()<<"replyFinished";
    if (reply->error() != QNetworkReply::NoError){
        qWarning() << "ERROR:" << reply->errorString();
        return;
    }
}

void networkAccess::loginGetReadyRead()
{
    qDebug()<<"slot get Ready Read";
//    qDebug()<<reply->readAll();

    QVariant variantCookies = reply->header(QNetworkRequest::SetCookieHeader);
    QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(variantCookies);
    qDebug() << "Cookies reply: " << cookies;

    manager.cookieJar()->setCookiesFromUrl(cookies, reply->request().url());
//    qDebug() << "Saved cookies: " << manager.cookieJar()->getAllCookies();
    qDebug() << "COOKIES " << manager.cookieJar()->cookiesForUrl(QUrl(LOGIN_URL));

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

void networkAccess::load(const QUrl&)
{

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

    QObject::connect(reply, SIGNAL(readyRead()), this, SLOT(mainPageReadyRead()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
}
