#include "OpenGLWidget.h"
#include <GL/glu.h>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {}

OpenGLWidget::~OpenGLWidget() {}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 1.0, 10000.0);
    glMatrixMode(GL_MODELVIEW);
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, -300, 300, 0, 0, 0, 0, 0, 1);

    QList<double> timeFrames = flightData.getAllTimeFrames();
    glPointSize(5.0f); // 设置点的大小
    glBegin(GL_POINTS);
    for (double t : timeFrames) {
        QMap<QString, ObjectState> states = flightData.getObjectStatesAt(t);
        for (const auto &state : states) {
            QVector3D pos = state.position;
            glColor3f(1.0f, 0.5f, 0.0f); // 可根据对象自定义颜色
            glVertex3f(pos.x(), pos.y(), pos.z());
        }
    }
    glEnd();
}


void OpenGLWidget::setFlightData(const FlightData &data) {
    flightData = data;
    update(); // 触发重绘
}