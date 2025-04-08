#include "MainWindow.h"
#include "OpenGLWidget.h"
#include "AcmiParser.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    glWidget = new OpenGLWidget(this);
    setCentralWidget(glWidget);
    setWindowTitle("TacViewLike");

    createMenus();
}

MainWindow::~MainWindow() {}

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu("&File");

    QAction *openAction = new QAction("&Open ACMI File", this);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenAcmiFile);
    fileMenu->addAction(openAction);

    // ✅ 添加播放按钮
    playAction = new QAction("&Play", this);
    playAction->setEnabled(false);  // 初始禁用
    connect(playAction, &QAction::triggered, [this]() {
        glWidget->startPlayback();  // 调用播放函数
    });
    fileMenu->addAction(playAction);

    QAction *exitAction = new QAction("&Exit", this);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);
}

void MainWindow::onOpenAcmiFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open ACMI File", "", "ACMI Files (*.txt *.acmi);;All Files (*)");
    if (!filePath.isEmpty()) {
        AcmiParser parser(filePath);
        if (parser.parse()) {
            flightData = parser.getFlightData();
            glWidget->setFlightData(flightData);
            QMessageBox::information(this, "ACMI Loaded", "Loaded file: " + filePath);

            playAction->setEnabled(true);  // ✅ 允许播放
        } else {
            QMessageBox::warning(this, "Error", "Failed to parse ACMI file.");
        }
    }
}
