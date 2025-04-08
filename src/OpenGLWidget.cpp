#include "OpenGLWidget.h"
#include <GL/glu.h>
#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {}

OpenGLWidget::~OpenGLWidget() {}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    earth.init();
}

void OpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, float(w) / float(h), 0.1, 100.0); // 注意 near 设为 0.1
    glMatrixMode(GL_MODELVIEW);
}


void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 相机视角由控制器设置
    cameraController.applyCamera();

    // 渲染地球球体（放大）
    glPushMatrix();
    glScalef(2.0f, 2.0f, 2.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, earth.getTextureId());
    earth.render();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // 渲染指南针（固定在右上角）
    drawCompass();
}



void OpenGLWidget::setFlightData(const FlightData &data) {
    flightData = data;
    update(); // 触发重绘
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
    cameraController.mousePress(event->x(), event->y());
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    cameraController.mouseMove(event->x(), event->y());
    update();  // 触发重绘
}

void OpenGLWidget::wheelEvent(QWheelEvent *event) {
    cameraController.wheelZoom(event->angleDelta().y());
    update();  // 触发重绘
}

void OpenGLWidget::drawCompass() {
    int w = width();
    int h = height();
    if (w <= 0 || h <= 0) return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    // 移动到右上角
    glTranslatef(w - 60, h - 60, 0);

    glRotatef(-cameraController.getRotationY(), 0, 0, 1);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 20);
    glVertex2f(-10, -10);
    glVertex2f(10, -10);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glColor3f(1.0f, 1.0f, 1.0f); // 在 drawCompass() 后恢复颜色
}
