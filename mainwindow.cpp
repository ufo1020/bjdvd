#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QWebElementCollection>
#include <QUrl>
#include <QString>
#include <QStringList>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "page.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //view = new QWebView(ui->mainPage);
    page = new QWebPage();
    frame = page->mainFrame();

//    frame->setHtml("<html><body><p>First Paragraph</p><p>Second Paragraph</p></body></html>");
//       QWebElement doc = frame->documentElement();
//       QWebElement body = doc.firstChild();

    connect(page, SIGNAL(loadFinished(bool)),this, SLOT(decode(bool)));
    loadPage(QUrl("file:///D:/Dev/Android/bjdvd/bjdvd.htm"));
}

MainWindow::~MainWindow()
{
    delete ui;
    if (view == nullptr) delete view;
    if (page == nullptr) delete page;
    view = nullptr;
    page = nullptr;
}

void MainWindow::loadPage(const QUrl& url)
{
    //view->load(QUrl("file:///D:/Dev/Android/bjdvd/bjdvd.htm"));
    frame->load(url);
}

void MainWindow::decode(bool ok)
{
     if (!ok){
         qDebug()<<"load not ok";
         return;
     }

    QWebElement main_element = frame->documentElement();
//    qDebug()<<main_element.toInnerXml();

/*
    QString subject;
    QString date;
    QString author;
    QString size;
    QString replies;
    QString clicks;
    QString body;

*/

    // generate page data
    QWebElementCollection posts = main_element.findAll("li[class='post active topic']");
    foreach (QWebElement post, posts) {
        QString subject = post.findFirst("div[class='post-subject']").
                                firstChild().firstChild().toPlainText();

        QWebElement post_facet = post.findFirst("div[class='post-facet']");

        QWebElement data = post_facet.firstChild();
        QWebElement author = data.nextSibling().firstChild();
        QWebElement size = author.nextSibling();
        QWebElement clicks = size.nextSibling();
        QWebElement replies = clicks.nextSibling();

        QStringList post_details;
        post_details<<subject.toCaseFolded()<<

        page_content.add_post();
        qDebug()<<title;
    }

//    view->show();

}
