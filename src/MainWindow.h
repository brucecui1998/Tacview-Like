#pragma once

#include <QMainWindow>
#include <QString>
#include "FlightData.h"

class OpenGLWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOpenAcmiFile();

private:
    void createMenus();

    OpenGLWidget *glWidget;
    FlightData flightData; // 数据模型
};