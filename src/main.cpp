#include <QApplication>
#include <QDebug>
#include "ui/MainWindow.h"

// 优先使用NVIDIA或AMD的显卡
extern "C" {
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
    _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.showMaximized(); 
    return app.exec();
}
