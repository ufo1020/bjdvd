#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QLabel>
#include <QMainWindow>
#include "common.h"
#include "page.h"

class QWebView;
class QWebPage;
class QWebFrame;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadPage(const QUrl& url);
    void initDisplayItems();

public slots:
    void decode(bool);

protected:
    bool event(QEvent* event) override final;

private:
    constexpr static int MAX_ITEMS = 30;
    constexpr static int MAX_CHARS_IN_LINE = 75;

    bool gestureEvent(QEvent* event);
    bool scrollEvent(QEvent* event);

    Ui::MainWindow *ui;
    QWebView* _view = nullptr;
    QWebPage* _page = nullptr;
    QWebFrame* _frame = nullptr;
    PageContent _page_content;
    std::vector<QLabel*> _items;
};

#endif // MAINWINDOW_H
