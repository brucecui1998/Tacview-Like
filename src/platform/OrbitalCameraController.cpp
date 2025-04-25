#include "windows.h"
#include "platform/OrbitalCameraController.h"
#include <GL/gl.h>
#include "OrbitalCameraController.h"

OrbitalCameraController::OrbitalCameraController()
    : distance(4.0f), yaw(180.0f), pitch(30.0f), sensitivity(0.3f), zoomSpeed(0.001f) {}

void OrbitalCameraController::mousePress(int x, int y, Qt::MouseButtons buttons) {
    lastPos = QPoint(x, y);
    currentButtons |= buttons; // 记录当前按下的所有按钮
}

void OrbitalCameraController::mouseRelease(int x, int y, Qt::MouseButtons buttons) {
    qDebug() << "[Camera] mouseRelease triggered. buttons:" << buttons;
    currentButtons = Qt::NoButton;

    qDebug() << "[Camera] After release, currentButtons:" << currentButtons;
}



void OrbitalCameraController::mouseMove(int x, int y, Qt::MouseButtons /*buttons*/) {
    qDebug() << "[Camera] mouseMove triggered. currentButtons:" << currentButtons;

    QPoint currentPos(x, y);
    QPoint delta = currentPos - lastPos;
    lastPos = currentPos;

    // ✅ 只处理单一按键
    if ((currentButtons & Qt::LeftButton) && !(currentButtons & Qt::RightButton)) {
        yaw += delta.x() * sensitivity;
        pitch += delta.y() * sensitivity;
        pitch = std::clamp(pitch, -89.0f, 89.0f);
        qDebug() << "[Camera] Left Drag - Yaw:" << yaw << "Pitch:" << pitch;
    }
    else if ((currentButtons & Qt::RightButton) && !(currentButtons & Qt::LeftButton)) {
        elevation += delta.y() * 0.02f;
        elevation = std::clamp(elevation, -5.0f, 5.0f);
        qDebug() << "[Camera] Right Drag - Elevation:" << elevation;
    }
    else {
        qDebug() << "[Camera] Ignored drag: ambiguous button state.";
    }
}



void OrbitalCameraController::wheelZoom(int delta) {
    float zoomFactor = std::pow(1.1f, -delta / 120.0f);
    distance *= zoomFactor;
    distance = std::clamp(distance, 1.2f, 20.0f);

    qDebug() << "[Camera] Zoom distance:" << distance;
}

void OrbitalCameraController::applyCamera() {
    float radYaw = qDegreesToRadians(yaw);
    float radPitch = qDegreesToRadians(pitch);

    // ✅ 基础相机视角
    QVector3D eye;
    eye.setX(distance * qCos(radPitch) * qSin(radYaw));
    eye.setY(distance * qSin(radPitch));
    eye.setZ(distance * qCos(radPitch) * qCos(radYaw));

    // ✅ 如果当前是右键按下状态，加入 elevation 偏移
    if (currentButtons & Qt::RightButton && !(currentButtons & Qt::LeftButton)) {
        eye.setY(eye.y() + elevation);  // ✅ elevation 只在右键控制时临时加上
    }

    QVector3D target(0, 0, 0);

    view.setToIdentity();
    view.lookAt(eye, target, QVector3D(0, 1, 0));
    eyePosition = eye;

    glLoadMatrixf(view.constData());

    qDebug() << "[Camera] Apply - Yaw:" << yaw
             << "Pitch:" << pitch
             << "Distance:" << distance
             << "Elevation (Applied?):" << ((currentButtons & Qt::RightButton) ? elevation : 0.0f);
}

QVector3D OrbitalCameraController::getEye() const {
    return eyePosition;
}

float OrbitalCameraController::getRotationY() const {
    return yaw;
}