#include "MainWindow.h"
#include "OpenGLWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    glWidget = new OpenGLWidget(this);
    setCentralWidget(glWidget);
    setWindowTitle("TacView-Like Minimal Demo");
}

MainWindow::~MainWindow() {}