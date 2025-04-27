#include "windows.h"
#include "platform/OrbitalCameraController.h"
#include <GL/gl.h>
#include "rendering/EarthSphere.h"
#include <GL/glu.h> // 新增

OrbitalCameraController::OrbitalCameraController()
    : distance(EarthSphere::EARTH_RADIUS * 5.0f),
      yaw(0.0f),
      pitch(45.0f), // 默认45度俯角
      sensitivity(0.2f),
      zoomSpeed(0.001f) {}

void OrbitalCameraController::setPitchAngle(float degrees) {
    pitch = std::clamp(degrees, -89.0f, 89.0f);
}

void OrbitalCameraController::mousePress(int x, int y, Qt::MouseButtons buttons) {
    lastPos = QPoint(x, y);
    currentButtons = buttons;
}

void OrbitalCameraController::mouseRelease(int x, int y, Qt::MouseButtons buttons) {
    currentButtons = Qt::NoButton;
}

void OrbitalCameraController::mouseMove(int x, int y, Qt::MouseButtons /*buttons*/) {
    if (currentButtons & Qt::LeftButton) {
        QPoint delta = QPoint(x, y) - lastPos;
        lastPos = QPoint(x, y);

        yaw -= delta.x() * sensitivity; // 水平旋转
        pitch += delta.y() * sensitivity; // 俯仰控制
        pitch = std::clamp(pitch, -89.0f, 89.0f);
        
        // 保持yaw在0-360范围内
        if (yaw > 360.0f) yaw -= 360.0f;
        if (yaw < 0.0f) yaw += 360.0f;
    }
}

// 修改缩放范围限制
void OrbitalCameraController::wheelZoom(int delta) {
    float zoomFactor = std::pow(1.1f, -delta / 120.0f);
    distance *= zoomFactor;
    distance = std::clamp(distance, 
                         EarthSphere::EARTH_RADIUS * 1.1f,  // 最小距离：1.1倍地球半径
                         EarthSphere::EARTH_RADIUS * 50.0f); // 最大距离：50倍地球半径
}

void OrbitalCameraController::applyCamera() {
    // 1. 设置视口（必须在窗口大小改变时更新）
    glViewport(0, 0, viewportWidth, viewportHeight);
    
    // 2. 计算相机位置（球坐标系转笛卡尔坐标）
    float radYaw = qDegreesToRadians(yaw);
    float radPitch = qDegreesToRadians(pitch);
    eyePosition.setX(distance * cos(radPitch) * sin(radYaw));
    eyePosition.setY(distance * sin(radPitch));
    eyePosition.setZ(distance * cos(radPitch) * cos(radYaw));

    // 3. 设置投影矩阵（关键修复点！）
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        45.0f, // 垂直视野角度
        (float)viewportWidth / viewportHeight, // 宽高比
        EarthSphere::EARTH_RADIUS * 0.1f, // 近裁剪面（避免裁切地球）
        EarthSphere::EARTH_RADIUS * 100.0f // 远裁剪面
    );

    // 4. 设置视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        eyePosition.x(), eyePosition.y(), eyePosition.z(), // 相机位置
        0, 0, 0,  // 观察目标（地球中心）
        0, 1, 0   // 上向量（Y轴向上）
    );

    // 5. 调试输出（发布时可移除）
    qDebug() << "Camera Position:" << eyePosition;
}

QVector3D OrbitalCameraController::getEye() const {
    return eyePosition;
}

float OrbitalCameraController::getRotationY() const {
    return pitch;
}

void OrbitalCameraController::setViewportSize(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
}