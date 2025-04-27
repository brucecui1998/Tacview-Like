#pragma once

#include "rendering/Config.h"
#include <QPoint>
#include <QMatrix4x4>
#include <QVector3D>

class MouseCameraController {
public:
    MouseCameraController();

    void mousePress(int x, int y, Qt::MouseButtons buttons);
    void mouseMove(int x, int y, Qt::MouseButtons buttons);
    void wheelZoom(int delta);
    void applyCamera();
    QVector3D getEye() const;
    float getRotationY() const;

private:
    QPoint lastPos;
    float rotX = 0.0f;
    float rotY = 0.0f;
    
    float distance = CAMERA_DEFAULT_DISTANCE;
    float distancePitch = 0.0f;

    bool leftButtonPressed = false;
    bool rightButtonPressed = false;
    // ✅ 控制灵敏度参数
    const float rotateSensitivity = 0.2f;        // 左键旋转灵敏度
    const float pitchSensitivity = 0.2f;       // 右键俯仰灵敏度
    const float zoomSensitivity = 0.0001f;        // 滚轮缩放灵敏度

    float rightDragElevation = 0.0f;  // ✅ 垂直俯视角，绕 X 或 Y 轴的额外旋转
};
