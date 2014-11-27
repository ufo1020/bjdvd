#include "mainwindow.h"
#include "datamanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataManager data_manager;
//    MainWindow w;
//    w.show();

    return a.exec();
}
