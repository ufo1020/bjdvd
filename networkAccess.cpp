#include "networkAccess.h"
#include "cookiejar.h"
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QNetworkRequest>
#include <QList>
#include <QVariant>
#include <QNetworkCookieJar>

networkAccess::networkAccess(QObject *parent, QWebFrame *frame) :
    QObject(parent),
    m_frame(frame),
    mLoginUrl("http://www.bjdvd.org/signin/"),
    mMainUrl("http://www.bjdvd.org/")
{
    m_cookies_jar = new CookieJar(this, mLoginUrl);
    manager.setCookieJar(m_cookies_jar);

    connect(&manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));

    // if not cookies found, login first, otherwise, uses cookies to open main page
    if (m_cookies_jar->is_empty()) {
        open_login();
    }else{
        open_main_page_with_cookies();
    }
}

networkAccess::~networkAccess()
{
    //save cookies to disk
    if (m_cookies_jar != nullptr) delete m_cookies_jar;
}

void networkAccess::open_login()
{
    QNetworkRequest request(mMainUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT_HEADER);
    reply = manager.get(request);
    mLoginState = STATES::GET_MAIN_PAGE;
}

// Request URL:http://www.bjdvd.org/
// Request Method:GET
// Status Code:200 OK
// Request Headers
//      Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
//      Accept-Encoding:gzip,deflate,sdch
//      Accept-Language:en,zh-CN;q=0.8,zh;q=0.6
//      Connection:keep-alive
//      Cookie:sessionid=c2f5f7e83fd799a32a42553eff559cf2; csrftoken=51190423f8430ac602b080b451f7d8c2
//      Host:www.bjdvd.org
//      User-Agent:Mozilla/5.0 (Windows NT 6.1; .....
void networkAccess::open_main_page_with_cookies()
{
    QNetworkRequest request(mMainUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT_HEADER);
    QList<QNetworkCookie> cookies = manager.cookieJar()->cookiesForUrl(mLoginUrl);
    for(QNetworkCookie cookie : cookies)
    {
        request.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookie));
    }

    reply = manager.get(request);

    if (m_frame) {
        m_frame->load(request);
    }

    mLoginState = STATES::DIRECT_MAIN_PAGE;
}

void networkAccess::replyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError){
        qWarning() << "ERROR:" << reply->errorString();
        return;
    }

    qDebug()<<"reply ready read "<<mLoginState;
    switch (mLoginState)
    {
    case STATES::UNKNOWN:
        break;
    case STATES::GET_MAIN_PAGE:
        get_main_page_ready_read();
        break;
    case STATES::REDIRECT_LOGIN_PAGE:
        redirect_login_page_ready_read();
        break;
    case STATES::LOGIN_POST:
        login_post_ready_read();
        break;
    case STATES::REDIRECT_MAIN_PAGE:
    case STATES::DIRECT_MAIN_PAGE:
        main_page_ready_read();
        break;
    default:
        break;
    }
}

// get main page will response 302 - redirect
// Response header:
//      Connection:keep-alive
//      Content-Language:zh-cn
//      Content-Type:text/html; charset=utf-8
//      Date:Tue, 04 Nov 2014 11:23:13 GMT
//      Location:http://www.bjdvd.org/signin/?next=/
//      Server:nginx
//      Transfer-Encoding:chunked
//      Vary:Accept-Language, Cookie
//      X-Static:miss
//
// Request header:
//      Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
//      Accept-Encoding:gzip,deflate,sdch
//      Accept-Language:en,zh-CN;q=0.8,zh;q=0.6
//      Connection:keep-alive
//      Host:www.bjdvd.org
//      User-Agent:Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko)
//                 Chrome/38.0.2125.111 Safari/537.36
void networkAccess::get_main_page_ready_read()
{
    qDebug()<<"slot get main page ready read";

    // handle redirect(302 request)
    QVariant location = reply->header(QNetworkRequest::LocationHeader);
    if (!location.isValid())
    {
        qDebug()<<"Location missing in reply, HALT...";
        return;
    }

    QNetworkRequest request(QUrl(location.toString()));
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT_HEADER);
    reply = manager.get(request);
    mLoginState = STATES::REDIRECT_LOGIN_PAGE;

    if (m_frame) {
        m_frame->load(request);
    }
}

// redirect to login page
// Response header:
//    Cache-Control:max-age=0
//    Connection:keep-alive
//    Content-Encoding:gzip
//    Content-Language:en
//    Content-Length:2428
//    Content-Type:text/html; charset=utf-8
//    Date:Tue, 04 Nov 2014 11:23:14 GMT
//    ETag:"40a2403c660d04a9d902685b5cd90288"
//    Expires:Tue, 04 Nov 2014 11:23:14 GMT
//    Last-Modified:Tue, 04 Nov 2014 11:23:14 GMT
//    Server:nginx
//    Set-Cookie:csrftoken=a47babca209db7507e37301974aea17b; Max-Age=31449600; Path=/
//    Set-Cookie:sessionid=78d97b4930fc1ec9324421bd6bd3bcc9; expires=Tue, 18-Nov-2014 11:23:14 GMT; Max-Age=1209600; Path=/
//    Vary:Cookie, Accept-Language, Accept-Encoding
//    X-Static:miss
//
// Request URL:http://www.bjdvd.org/signin/
// Request Method:POST
// Status Code:302 FOUND
// Request header:
//    Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
//    Accept-Encoding:gzip,deflate
//    Accept-Language:en,zh-CN;q=0.8,zh;q=0.6
//    Cache-Control:max-age=0
//    Connection:keep-alive
//    Content-Length:139
//    Content-Type:application/x-www-form-urlencoded
//    Cookie:csrftoken=a47babca209db7507e37301974aea17b; sessionid=78d97b4930fc1ec9324421bd6bd3bcc9
//    Host:www.bjdvd.org
//    Origin:http://www.bjdvd.org
//    Referer:http://www.bjdvd.org/signin/?next=/
//    User-Agent:Mozilla/5.0 (Windows NT 6.1; WOW64).......
//    Form Data:
//          csrfmiddlewaretoken:a47babca209db7507e37301974aea17b
//          username:xxxx
//          password:xxxx
//          remember_me:on
//          next:/

void networkAccess::redirect_login_page_ready_read()
{
    qDebug()<<"slot redirect login page ready read";

    QVariant variantCookies = reply->header(QNetworkRequest::SetCookieHeader);
    QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(variantCookies);
    manager.cookieJar()->setCookiesFromUrl(cookies, mLoginUrl);
    qDebug() << "Cookies reply: " << cookies;

    QNetworkRequest request(mLoginUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader, 139);

    QString csrfmiddlewaretoken;
    // set cookies
    for(QNetworkCookie cookie : cookies)
    {
        // get csrftoken value
        if(cookie.name() == QByteArray("csrftoken"))
        {
            csrfmiddlewaretoken = cookie.value();
        }
        request.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookie));
//        qDebug() << "POST Header COOKIES " << cookie;
    }

    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT_HEADER);

    if (csrfmiddlewaretoken.size() == 0)
    {
        qDebug()<<"csrfmiddlewaretoken WRONG!!! halt...";
        return;
    }
    QByteArray postData;
    postData.append("csrfmiddlewaretoken=");
    postData.append(csrfmiddlewaretoken);
    postData.append("&");
    postData.append("username=%E5%8D%95%E7%A8%8B%E5%B7%B4%E5%A3%AB&");
    postData.append("password=3232917&");
    postData.append("remember_me=on&next=%2F");

    reply = manager.post(request, postData);
    mLoginState = STATES::LOGIN_POST;
}

// Ready to open the main page
// Response header:
//      Cache-Control:max-age=0
//      Connection:keep-alive
//      Content-Language:en
//      Content-Type:text/html; charset=utf-8
//      Date:Tue, 04 Nov 2014 12:13:20 GMT
//      ETag:"d41d8cd98f00b204e9800998ecf8427e"
//      Expires:Tue, 04 Nov 2014 12:13:20 GMT
//      Last-Modified:Tue, 04 Nov 2014 12:13:20 GMT
//      Location:http://www.bjdvd.org/
//      Server:nginx
//      Set-Cookie:sessionid=a7424c250a9e5a2bd3e3cb10c34fc7c3; expires=Tue, 18-Nov-2014 12:13:20 GMT; Max-Age=1209600; Path=/
//      Transfer-Encoding:chunked
//      Vary:Accept-Language, Cookie
//      X-Static:miss
//
// Request URL:http://www.bjdvd.org/
// Request Method:GET
// Status Code:200 OK
// Request Headers:
//      Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
//      Accept-Encoding:gzip,deflate,sdch
//      Accept-Language:en,zh-CN;q=0.8,zh;q=0.6
//      Cache-Control:max-age=0
//      Connection:keep-alive
//      Cookie:csrftoken=a47babca209db7507e37301974aea17b; sessionid=a7424c250a9e5a2bd3e3cb10c34fc7c3
//      Host:www.bjdvd.org
//      Referer:http://www.bjdvd.org/signin/?next=/
//      User-Agent:Mozilla/5.0 (Windows NT 6.1; WOW64) ....
//      Response Headersview source
void networkAccess::login_post_ready_read()
{
    qDebug()<<"slot login post Ready Read";

    QNetworkRequest request(mMainUrl);

    // set-cookie update it into Cookiejar
    QVariant variantCookies = reply->header(QNetworkRequest::SetCookieHeader);
    // new cookies should only has session id
    QList<QNetworkCookie> new_cookies_sessionid = qvariant_cast<QList<QNetworkCookie> >(variantCookies);
    qDebug() << "Cookies reply: " << new_cookies_sessionid;
    Q_ASSERT(new_cookies_sessionid.size() == 1);

    // old cookies has token and session id
    QList<QNetworkCookie> stored_cookies = manager.cookieJar()->cookiesForUrl(mLoginUrl);
    // delete session id from old cookies
    for(QNetworkCookie cookie : stored_cookies)
    {
        if(cookie.name() == QByteArray("sessionid"))
        {
            if (!manager.cookieJar()->deleteCookie(cookie)) {
                qDebug()<<"delete session id failed HALT.....";
                return;                ;
            }
        }
    }

    // insert session id in old cookies

    for(QNetworkCookie cookie : new_cookies_sessionid)
    {
        stored_cookies.append(cookie);
//        if (!manager.cookieJar()->insertCookie(cookie)) {
//            qDebug()<<"update session id failed HALT.....";
//            return;
//        }
    }

    // set csrftoken from cookie Jar, set sessionid from last response
//    stored_cookies = manager.cookieJar()->cookiesForUrl(mLoginUrl);
    for(QNetworkCookie cookie : stored_cookies)
    {
//        if(cookie.name() == QByteArray("csrftoken"))
//        {
            request.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookie));
//            break;
//        }
            qDebug()<<"new cookies send: "<<cookie;
    }
//    request.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(sessionid));

    manager.cookieJar()->setCookiesFromUrl(stored_cookies, mLoginUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT_HEADER);
    reply = manager.get(request);
    mLoginState = STATES::REDIRECT_MAIN_PAGE;
}

void networkAccess::main_page_ready_read()
{
    qDebug()<<"main page!!!!! "<<reply->readAll();
}

void networkAccess::slotError(QNetworkReply::NetworkError e)
{
    qDebug()<<"slotError "<< e;
}

