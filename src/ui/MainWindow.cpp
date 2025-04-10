#include "MainWindow.h"
#include "OpenGLWidget.h"
#include "core/AcmiParser.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    glWidget = new OpenGLWidget(this);
    setCentralWidget(glWidget);
    setWindowTitle("TacviewLike");
    resize(1024, 768);
    createMenus();

}

MainWindow::~MainWindow() {}

void MainWindow::createMenus() {
    QMenu* fileMenu = menuBar()->addMenu("&File");

    QAction* openAction = new QAction("&Open ACMI File", this);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenAcmiFile);
    fileMenu->addAction(openAction);

    playAction = new QAction("&Play", this);
    playAction->setEnabled(false);
    connect(playAction, &QAction::triggered, [this]() {
        glWidget->startPlayback();
    });
    fileMenu->addAction(playAction);

    // ✅ 添加退出菜单项
    QAction* exitAction = new QAction("&Exit", this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
}

void MainWindow::onOpenAcmiFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open ACMI File", "", "ACMI Files (*.txt *.acmi)");
    if (filePath.isEmpty()) return;

    AcmiParser* parser = new AcmiParser(filePath);
    if (!parser->parse()) {
        QMessageBox::warning(this, "Error", "Failed to parse ACMI file.");
        delete parser;
        return;
    }

    glWidget->setFlightDataProvider(parser);
    playAction->setEnabled(true);
    QMessageBox::information(this, "ACMI Loaded", "Loaded file: " + filePath);
}

