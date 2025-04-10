#include <windows.h>
#include "MouseCameraController.h"
#include <GL/glu.h>
#include <QtMath>

MouseCameraController::MouseCameraController() {}

void MouseCameraController::mousePress(int x, int y, Qt::MouseButtons buttons) {
    lastPos = QPoint(x, y);
}

void MouseCameraController::mouseMove(int x, int y, Qt::MouseButtons buttons) {
    int dx = x - lastPos.x();
    int dy = y - lastPos.y();
    lastPos = QPoint(x, y);

    if (buttons & Qt::LeftButton) {
        rotX += dy * rotateSensitivity;
        rotY += dx * rotateSensitivity;
        rotX = qBound(-89.9f, rotX, 89.9f);
    } else if (buttons & Qt::RightButton) {
        rightDragElevation += dy * pitchSensitivity;
        rightDragElevation = qBound(-89.0f, rightDragElevation, 89.0f);
    }
}

void MouseCameraController::wheelZoom(int delta) {
    float scaledDelta = delta / 120.0f; // 一格滚轮为 120，标准化
    distance *= std::pow(1.1f, -scaledDelta); // 缩放因子更柔和
    distance = qBound(1.1f, distance, 5.0f);
}

void MouseCameraController::applyCamera() {
    QVector3D eye(0, -distance, 0);

    QMatrix4x4 mat;
    mat.rotate(rotY, 0, 0, 1);                    // 左右旋转（水平）
    mat.rotate(rotX, 1, 0, 0);                    // 上下旋转（仰俯）
    mat.rotate(rightDragElevation, 1, 0, 0);      // ✅ 右键控制的仰角

    QVector3D rotatedEye = mat * eye;

    gluLookAt(rotatedEye.x(), rotatedEye.y(), rotatedEye.z(),
              0, 0, 0,
              0, 0, 1);
}

QVector3D MouseCameraController::getEye() const {
    QVector3D eye(0, -distance, 0);
    QMatrix4x4 mat;
    mat.rotate(rotY, 0, 0, 1);
    mat.rotate(rotX, 1, 0, 0);
    mat.rotate(rightDragElevation, 1, 0, 0);
    return mat * eye;
}

float MouseCameraController::getRotationY() const {
    return rotY;
}

