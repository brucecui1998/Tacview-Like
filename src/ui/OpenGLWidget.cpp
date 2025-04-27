#include "OpenGLWidget.h"
#include <QOpenGLFunctions>  // 为了 initializeOpenGLFunctions
#include <GL/glu.h>      // for gluPerspective
#include <QtMath>        // for qDegreesToRadians


OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);

    connect(&playbackTimer, &QTimer::timeout, this, [this]() {
        if (frameTimes.isEmpty()) return;
        double elapsed = playbackClock.elapsed() / 1000.0;
        while (frameIndex + 1 < frameTimes.size() && frameTimes[frameIndex + 1] <= elapsed) {
            ++frameIndex;
        }
        currentTime = frameTimes[frameIndex];
        scene.updateCurrentTime(currentTime);

        if (frameIndex >= frameTimes.size() - 1) {
            playbackTimer.stop();
        }
        update();
    });

    QTimer::singleShot(0, this, [this]() {
        update();
    });
}

// UI 控制层负责创建和提供数据模块的实例，但不直接调用数据模块的接口。
// 它将控制权下放给 SceneManager，自己只通过 SceneManager 间接访问数据。
void OpenGLWidget::setFlightDataProvider(IFlightDataProvider* provider) {
    scene.bindDataProvider(provider);
    frameTimes = scene.getTimeFrames();
    frameIndex = 0;
    currentTime = 0.0;
    update();
}

void OpenGLWidget::startPlayback() {
    if (frameTimes.isEmpty()) return;
    frameIndex = 0;
    currentTime = frameTimes[0];
    playbackClock.restart();
    playbackTimer.start(33);
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);    // ✅ 启用深度缓冲
    glDisable(GL_CULL_FACE);    // ✅ 禁用背面剔除，防止翻转被剔除

    qDebug() << "OpenGL Version:" << reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << "GLSL Version:" << reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    qDebug() << "Vendor:" << reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    qDebug() << "Renderer:" << reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    renderer.initialize();
}


void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera.applyCamera();     // ✅ 设置视角矩阵

    renderer.renderEarth();             // ✅ 必须放这（放这里才有视角效果）
    // renderer.renderScene(scene);  // ✅ 绘制飞机、目标等

    QVector3D eye = camera.getEye();
    renderer.renderScene(scene, eye);

    //hudRenderer.drawCompass(camera.getRotationY(), width(), height());
}


void OpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0, float(w) / float(h), 0.1, 100000.0);  // ⬅ 更广角度 + 远截面支持
    glMatrixMode(GL_MODELVIEW);
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event) {
    camera.mousePress(event->pos().x(), event->pos().y(), event->buttons());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {
    camera.mouseRelease(event->pos().x(), event->pos().y(), event->buttons());
}


void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
    camera.mouseMove(event->x(), event->y(), event->buttons());  // ✅ 传入按钮状态
    update();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event) {
    camera.wheelZoom(event->angleDelta().y());
    update();
}
