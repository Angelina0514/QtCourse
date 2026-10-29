#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("小牛文本编辑器");
    a.setApplicationVersion("1.0.0");

    MainWindow w;
    w.show();

    return a.exec();
}
