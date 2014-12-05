
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QWebElementCollection>
#include <QSizePolicy>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QGestureEvent>
#include <QScrollEvent>
#include <QScroller>
#include <QRegExp>
#include <QScrollBar>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_login.h"
#include "page.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _main_ui(new Ui::MainWindow),
    _login_ui(new Ui::Login)
{
    _main_ui->setupUi(this);
    _login_ui->setupUi(this);

    _main_ui->scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar {width:0px;}");

    // = new QWeb(ui->mainPage);
    _page = new QWebPage();
    _frame = _page->mainFrame();

    connect(_page, SIGNAL(loadFinished(bool)),this, SLOT(decode(bool)));
//    loadPage(QUrl("file:///D:/Dev/Android/bjdvd/bjdvd.htm"));
    m_networkManager = new networkAccess(this, _frame);

    for(int i = 0; i < MAX_ITEMS; i++)
    {
        QLabel* item = new QLabel();
        item->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum));
        item->setFixedWidth(_main_ui->contents->width());
        item->setMinimumHeight(30);
        item->setWordWrap(true);
        _items.push_back(item);
        _main_ui->verticalLayout->addWidget(item);
    }
//    ui->contents->adjustSize();
    qDebug()<<"h:"<<_main_ui->contents->height()<<" w:"<<_main_ui->contents->width();

}


MainWindow::~MainWindow()
{
    delete _main_ui;
    if (_view == nullptr) delete _view;
    if (_page == nullptr) delete _page;
    delete m_networkManager;

    foreach(QLabel* label, _items)
    {
        if (label != nullptr) delete label;
        label = nullptr;
    }


    _view = nullptr;
    _page = nullptr;
}

void MainWindow::loadPage(const QUrl& url)
{
    _frame->load(url);
}

void MainWindow::decode(bool ok)
{
     if (!ok){
         qDebug()<<"load not ok";
         return;
     }

    QWebElement main_element = _frame->documentElement();
    qDebug()<<main_element.toInnerXml();


    // generate page data
    QWebElementCollection posts = main_element.findAll("li[class='post active topic']");
    qDebug()<<"subjects:"<<posts.count();
    foreach (QWebElement post, posts) {
        QString subject = post.findFirst("div[class='post-subject']").
                                firstChild().firstChild().toPlainText();

        QWebElement post_facet = post.findFirst("div[class='post-facet']");

        QWebElement data = post_facet.firstChild();
        QWebElement author = data.nextSibling().firstChild();
        QWebElement size = author.nextSibling();
        QWebElement clicks = size.nextSibling();
        QWebElement replies = clicks.nextSibling();
        QWebElement body = post.findFirst("div[class='post-body']").firstChild();

        QStringList post_details;
        post_details<<subject<<data.toPlainText()<<author.toPlainText()
                   <<size.toPlainText()<<clicks.toPlainText()<<replies.toPlainText()
                   <<body.toPlainText();

        _page_content.add_post(PostDetails(post_details));
        //qDebug()<<title;
    }

    initDisplayItems();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    qDebug()<<"here...";
}

bool MainWindow::scrollEvent(QEvent* event)
{
    QScrollEvent* e = static_cast<QScrollEvent*>(event);


    static int y_offset = 0;
//    QScroller *scroller = QScroller::scroller(ui->centralWidget);
//    scroller->scrollTo(QPointF(0, y_offset));
//    ui->centralWidget->move(0, y_offset);
    _main_ui->contents->scroll(0,100);
    y_offset += 100;
    qDebug()<<"scrolling y:"<<y_offset;
//    qDebug()<<y_offset;
//    qDebug()<<ui->mainPage->height();
//    qDebug()<<ui->mainPage->width();
//    this->update();
}

bool MainWindow::gestureEvent(QEvent* event)
{
    QGestureEvent* e = static_cast<QGestureEvent*>(event);

//    if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
//        swipeTriggered(static_cast<QSwipeGesture *>(swipe));
//    else if (QGesture *pan = event->gesture(Qt::PanGesture))
//        panTriggered(static_cast<QPanGesture *>(pan));
//    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
//        pinchTriggered(static_cast<QPinchGesture *>(pinch));
//    return true;
}

void MainWindow::initDisplayItems()
{
    std::vector<PostDetails> posts = _page_content.get_posts();
    int y_offset = 0;
    int item_count = posts.size() > _items.size()?_items.size():posts.size();
    for(int i = 0; i < item_count; i++)
    {
        PostDetails details = posts[i];
//        _items[i]->setGeometry(QRect(0, y_offset, 480, 100));

        QString content = (details.get_subject());

        content.append("\n");
        content.append((details.get_body()));
//        qDebug() << content;
        _items[i]->setText(content);

        y_offset += 100;
    }
    _main_ui->contents->adjustSize();
    qDebug()<<"h:"<<_main_ui->contents->height()<<" w:"<<_main_ui->contents->width();
    emit QPaintEvent(_main_ui->contents->rect());
}

