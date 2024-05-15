#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen splash;
    MainWindow w;

    // splash.setPixmap(QPixmap(":/graphics/splash.jpg"));
    // splash.show();

    // QTimer::singleShot(8000, &splash, SLOT(close()));
    // QTimer::singleShot(9000, &w, SLOT(show()));
    w.show();
    return a.exec();
}
