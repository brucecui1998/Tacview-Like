#include <windows.h>
#include "MouseCameraController.h"
#include <GL/glu.h>
#include <QtMath>

MouseCameraController::MouseCameraController() {}

void MouseCameraController::mousePress(int x, int y) {
    lastPos = QPoint(x, y);
}

void MouseCameraController::mouseMove(int x, int y) {
    int dx = x - lastPos.x();
    int dy = y - lastPos.y();
    lastPos = QPoint(x, y);

    const float sensitivity = 0.2f;
    rotX += dy * sensitivity;
    rotY += dx * sensitivity;
    rotX = qBound(-89.9f, rotX, 89.9f);
}

void MouseCameraController::wheelZoom(int delta) {
    const float zoomSensitivity = 0.0005f;
    distance -= delta * zoomSensitivity;
    distance = qBound(1.0f, distance, 100.0f);  // 增大最大范围
}

void MouseCameraController::applyCamera() {
    // ✅ 修改为更远的默认距离，视野更广
    QVector3D eye(0, -distance, distance);  // 改为更远离球体

    QMatrix4x4 mat;
    mat.rotate(rotY, 0, 0, 1);
    mat.rotate(rotX, 1, 0, 0);
    QVector3D rotatedEye = mat * eye;

    gluLookAt(rotatedEye.x(), rotatedEye.y(), rotatedEye.z(),
              0, 0, 0,
              0, 0, 1);
}

float MouseCameraController::getRotationY() const {
    return rotY;
}