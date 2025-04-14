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
        // 左键保持原有轨道控制
        rotX += dy * rotateSensitivity;
        rotY += dx * rotateSensitivity;
        rotX = qBound(-89.9f, rotX, 89.9f);
    } else if (buttons & Qt::RightButton) {
        // ✅ 仅高度变化（垂直方向控制）
        heightOffset -= dy * verticalMoveSensitivity;
        heightOffset = qBound(CAMERA_MIN_DISTANCE, heightOffset, CAMERA_MAX_DISTANCE); 

        // ❌ 去除俯仰和倾斜控制（不再改变 rightDragPitch / rollAngle）
        rightDragPitch += dy * pitchSensitivity * pitchContribution;
        rightDragPitch = qBound(-180.0f, rightDragPitch, 180.0f);

        rollAngle += dx * rollSensitivity;
        rollAngle = qBound(-180.0f, rollAngle, 180.0f);

        distance -= dy * moveSensitivity; // 前后移动（保留也可以，或你也可以注释掉）
        distance = qBound(CAMERA_MIN_DISTANCE, distance, CAMERA_MAX_DISTANCE);

        // ✅ 调试输出
        qDebug() << "[MouseCamera] Right drag:"
                 << "dx:" << dx << "dy:" << dy
                 << "distance:" << distance
                 << "heightOffset:" << heightOffset
                 << "rollAngle: " << rollAngle
                 << "rightDragPitch: " << rightDragPitch;
    }
}


void MouseCameraController::wheelZoom(int delta) {
    float scaledDelta = delta / 120.0f;
    distance *= std::pow(1.1f, -scaledDelta);
    distance = qBound(-30000.0f, distance, 30000.0f);
}

void MouseCameraController::applyCamera() {
    QVector3D eye(0, -distance, heightOffset); // 加入高度偏移

    QMatrix4x4 mat;
    // 调整旋转顺序保证运动自然
    mat.rotate(rotY, 0, 0, 1);       // 水平旋转
    mat.rotate(rotX, 1, 0, 0);       // 轨道俯仰
    mat.rotate(rightDragPitch, 1, 0, 0); // 俯仰视角
    mat.rotate(rollAngle, 0, 0, 1);  // 倾斜控制

    // 计算最终位置和上方向
    QVector3D rotatedEye = mat * eye;
    QVector3D up = mat * QVector3D(0, 0, 1);

    gluLookAt(
        rotatedEye.x(), 
        rotatedEye.y(), 
        rotatedEye.z(),  // Z轴包含高度偏移
        0, 0, 0,         // 保持看向原点
        up.x(), up.y(), up.z()
    );
}
QVector3D MouseCameraController::getEye() const {
    QVector3D eye(0, -distance, 0);
    QMatrix4x4 mat;
    mat.rotate(rotY, 0, 0, 1);
    mat.rotate(rotX, 1, 0, 0);
    mat.rotate(rightDragPitch, 1, 0, 0);
    mat.rotate(rollAngle, 0, 0, 1);
    return mat * eye;
}

float MouseCameraController::getRotationY() const {
    return rotY;
}