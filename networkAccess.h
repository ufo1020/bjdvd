#ifndef NETWORKACCESS_H
#define NETWORKACCESS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkCookie>

class networkAccess : public QObject
{
    Q_OBJECT

public:
    networkAccess(QObject *parent = 0);
    ~networkAccess();

    void loginGet();
    // first step of login, send get request, returns cookies
    void loginGetCookies();
    // second step of login, get cooikes from server, then post id and password to server
    void loginPostPassword();
    void loginDone();

    // load page after login
    void load(const QUrl&);
    void redirect(const QUrl&);


    void loginGetReadyRead();
    void loginPostReadyRead();
    void redirectReadyRead();
    void mainPageReadyRead();
public slots:
    void replyFinished(QNetworkReply*);
    void slotError(QNetworkReply::NetworkError);



private:
    enum STATES {
        UNKNOWN = 0,
        LOGIN_GET,
        LOGIN_POST,
        REDIRECT
    };

    QNetworkAccessManager manager;
    QNetworkReply* reply;
    QUrl mLoginUrl;
    QUrl mMainUrl;
    int mLoginState = STATES::UNKNOWN;
//    QList<QNetworkCookie> cookies;
};

#endif // NETWORKACCESS_H
