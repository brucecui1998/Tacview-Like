#include <QApplication>
#include <QDebug>
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.showMaximized(); 
    return app.exec();
}
