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
    float rotX = 0.0f;        // 轨道俯仰（左键控制）
    float rotY = 0.0f;        // 水平旋转（左键控制）
    
    float distance = CAMERA_DEFAULT_DISTANCE;
    float rightDragPitch = 0.0f;  // 右键俯仰
    float rollAngle = 0.0f;       // 倾斜角度

    bool leftButtonPressed = false;
    bool rightButtonPressed = false;

    // 控制参数
    const float rotateSensitivity = 0.2f;    // 左键旋转
    const float moveSensitivity = 50.0f;     // 前后移动
    const float rollSensitivity = 2.0f;      // 倾斜灵敏度
    const float zoomSensitivity = 0.0001f;   // 滚轮缩放
    // 新增高度偏移量控制
    float heightOffset = 0.0f; // 相机高度偏移量（垂直移动）
    // 调整灵敏度参数
    const float verticalMoveSensitivity = 20.0f;  // 高度灵敏度大幅提升
    const float pitchSensitivity = 6.0f;         // 俯仰基础灵敏度
    const float pitchContribution = 0.2f;         // 俯仰贡献比例降低
    

};