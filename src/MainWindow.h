#pragma once

#include <QMainWindow>

class OpenGLWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    OpenGLWidget *glWidget;
};