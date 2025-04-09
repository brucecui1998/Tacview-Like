#include "OpenGLWidget.h"
#include <GL/glu.h>
#include <QDebug>
#include <QtMath>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);  // 保证滚轮等事件能触发

    // ✅ 只绑定播放定时器，不启动
    connect(&playbackTimer, &QTimer::timeout, this, [this]() {
        if (frameTimes.isEmpty()) return;
    
        double elapsed = playbackClock.elapsed() / 1000.0; // 秒
    
        // ✅ 自动推进到正确帧
        while (frameIndex + 1 < frameTimes.size() && frameTimes[frameIndex + 1] <= elapsed) {
            ++frameIndex;
        }
    
        // ✅ 获取当前帧对应时间（可用于渲染）
        currentTime = frameTimes[frameIndex];
    
        // ✅ 自动停止播放（可改为循环）
        if (frameIndex >= frameTimes.size() - 1) {
            playbackTimer.stop(); // 或 loop: frameIndex = 0;
        }
    
        update(); // 触发 repaint
    });
    

    // ✅ 初始化后强制刷新一次，避免黑屏
    QTimer::singleShot(0, this, [this]() {
        update();
    });
}




OpenGLWidget::~OpenGLWidget() {}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    earth.init();
    if (!f16Model.load("resources/models/FixedWing.F-16.obj")) {
        qWarning() << "[Model] Failed to load F-16!";
    }
    else {
        qDebug() << "[Model] F-16 loaded successfully!";
    }

}

void OpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, float(w) / float(h), 0.1, 10.0); // zFar ≥ 2.0 最稳
    glMatrixMode(GL_MODELVIEW);
}

QVector3D geoToSphere(double latDeg, double lonDeg, double radius = 1.0) {
    double lat = qDegreesToRadians(latDeg);
    double lon = qDegreesToRadians(lonDeg);
    return QVector3D(
        radius * cos(lat) * cos(lon),
        radius * cos(lat) * sin(lon),
        radius * sin(lat)
    );
}



void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 相机视角由控制器设置
    cameraController.applyCamera();

    // 渲染地球球体（放大）
    glPushMatrix();
    //glScalef(2.0f, 2.0f, 2.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, earth.getTextureId());
    earth.render();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // 渲染指南针（固定在右上角）
    drawCompass();

    glPushMatrix();

    // 经纬度转球面
    QMap<QString, ObjectState> stateMap = flightData.getObjectStatesAt(currentTime);
    if (stateMap.contains("A0100")) { // 示例ID（你可以遍历所有 ID）
        const ObjectState &state = stateMap["A0100"];

         // 直接打印原始 ACMI 值
        qDebug() << "[ACMI] Pos (Lon, Lat, Alt):"
        << state.position.x() << state.position.y() << state.position.z();

        // 经纬度转球面坐标（假设state.position存的是经度/纬度/高度）
        double lon = state.position.x(); // 经度
        double lat = state.position.y(); // 纬度
        double alt = state.position.z(); // 海拔（可用于调节距离地表的比例）

        QVector3D pos = geoToSphere(lat, lon, 1.0 + alt / 100000.0);
        qDebug() << "[geoToSphere] Mapped pos =" << pos;
        // 同时打印姿态角（可选）
            qDebug() << "[ACMI] Pitch:" << state.pitch
            << "Roll:" << state.roll
            << "Yaw:" << state.yaw;
        glPushMatrix();
        glTranslatef(pos.x(), pos.y(), pos.z());

        // 方向姿态角（单位：度）
        glRotatef(state.yaw,   0, 0, 1); // 偏航（绕地球 Z轴旋转）
        glRotatef(state.pitch, 1, 0, 0); // 俯仰（绕飞机 X轴旋转）
        glRotatef(state.roll,  0, 1, 0); // 翻滚（绕飞机 Y轴旋转）

        // 模型朝向修正
        glRotatef(-90, 1, 0, 0); // 机头从 +Z 转向 +X
        glRotatef(180, 0, 0, 1); // 机腹朝上 → 翻个身

        // 缩放
        glScalef(0.005f, 0.005f, 0.005f);
        
        glPushAttrib(GL_CURRENT_BIT);         // ✅ 保存当前颜色状态
        glColor3f(1.0f, 0.0f, 0.0f);          // ✅ 红色飞机
        f16Model.render();
        glPopAttrib();                        // ✅ 恢复之前颜色

        glPopMatrix();
    }
}



void OpenGLWidget::setFlightData(const FlightData &data) {
    flightData = data;
    frameTimes = flightData.getAllTimeFrames();
    frameIndex = 0;
    currentTime = 0.0;

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
    update();
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

void OpenGLWidget::startPlayback() {
    if (frameTimes.isEmpty()) return;

    frameIndex = 0;
    currentTime = frameTimes[0]; // 从第一帧开始
    playbackClock.restart();     // ✅ 启动高精度定时器
    playbackTimer.start(33);     // 每 33ms 检查是否推进帧（可调）
}

