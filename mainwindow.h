#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common.h"

class QWebView;
class QWebPage;
class QWebFrame;
class PageContent;

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

public slots:
    void decode(bool);

private:

    Ui::MainWindow *ui;
    QWebView* view = nullptr;
    QWebPage* page = nullptr;
    QWebFrame* frame = nullptr;
    PageContent page_content;
};

#endif // MAINWINDOW_H
