#pragma once

#include <QMainWindow>
#include <QString>
class OpenGLWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onOpenAcmiFile();

private:
    void createMenus();

    OpenGLWidget* glWidget;
    QAction* playAction = nullptr;
};