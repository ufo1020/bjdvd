#ifndef NETWORKACCESS_H
#define NETWORKACCESS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkCookie>

class CookieJar;

class networkAccess : public QObject
{
    Q_OBJECT

public:
    networkAccess(QObject *parent = 0);
    ~networkAccess();

    // send request to open bjdvd page
    void open_login();
    void open_main_page_with_cookies();

    // first step of login, send get request, returns cookies
    void loginGetCookies();
    void loginDone();

    // load page after login
    void load(const QUrl&);

    // Reply ready to read
    void get_main_page_ready_read();
    void redirect_login_page_ready_read();
    void login_post_ready_read();
    void redirect_main_page_ready_read();
    void direct_main_page_ready_read();
    void mainPageReadyRead();
public slots:
    void replyFinished(QNetworkReply*);
    void slotError(QNetworkReply::NetworkError);



private:
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
    constexpr static char const* USER_AGENT_HEADER = "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 \
                                           (KHTML, like Gecko) Chrome/37.0.2062.124 Safari/537.36";
    enum STATES {
        UNKNOWN = 0,
        GET_MAIN_PAGE,
        REDIRECT_LOGIN_PAGE,
        LOGIN_POST,
        REDIRECT_MAIN_PAGE,
        DIRECT_MAIN_PAGE   //direct open main page with cookies stored before
    };

    QNetworkAccessManager manager;
    QNetworkReply* reply = nullptr;
    CookieJar* m_cookies_jar = nullptr;
    QUrl mLoginUrl;
    QUrl mMainUrl;
    int mLoginState = STATES::UNKNOWN;
//    QList<QNetworkCookie> cookies;
};

#endif // NETWORKACCESS_H
