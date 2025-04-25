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
        rotX = qBound(-179.9f, rotX, 179.9f);
        //qDebug() << "[mouseMove] rotX is " << rotX;
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
    mat.rotate(rotY, 0, 0, 1);
    mat.rotate(rotX, 1, 0, 0);
    mat.rotate(rightDragElevation, 1, 0, 0);

    QVector3D rotatedEye = mat * eye;
    QVector3D center(0, 0, 0);
    QVector3D viewDir = (center - rotatedEye).normalized();

    QVector3D baseUp(0, 1, 0);
    QVector3D right = QVector3D::crossProduct(viewDir, baseUp).normalized();

    if (right.lengthSquared() < 1e-6f) {
        baseUp = QVector3D(0, 0, 1);
        right = QVector3D::crossProduct(viewDir, baseUp).normalized();
    }

    QVector3D up = QVector3D::crossProduct(right, viewDir).normalized();

    // 🔍 Debug 输出
    qDebug() << "[Camera] rotX:" << rotX << "rotY:" << rotY << "distance:" << distance;
    qDebug() << "[Camera] Eye:" << rotatedEye;
    qDebug() << "[Camera] ViewDir:" << viewDir;
    qDebug() << "[Camera] Up:" << up;

    gluLookAt(rotatedEye.x(), rotatedEye.y(), rotatedEye.z(),
              center.x(), center.y(), center.z(),
              up.x(), up.y(), up.z());
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

