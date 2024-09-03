#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/dark/stylesheet.qss"); //установка темной темы
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    a.setStyleSheet(stream.readAll());
    file.close();
    MainWindow w;
    w.hide();
    return a.exec();
}
