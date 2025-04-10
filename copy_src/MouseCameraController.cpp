#include "MouseCameraController.h"
#include <windows.h>
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
    const float zoomSensitivity = 0.002f; // 
    distance -= delta * zoomSensitivity;

    distance = qBound(0.5f, distance, 50.0f); // 限制缩放范围
}


void MouseCameraController::applyCamera() {
    QVector3D eye(0, -distance, distance * 0.6f);
    QMatrix4x4 mat;
    mat.rotate(rotY, 0, 0, 1);
    mat.rotate(rotX, 1, 0, 0);

    QVector3D rotatedEye = mat * eye;

    gluLookAt(rotatedEye.x(), rotatedEye.y(), rotatedEye.z(),
            0, 0, 0,
            0, 0, 1);
}