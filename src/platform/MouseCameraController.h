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
    QVector3D calculateUpVector();
    QVector3D calculateRightClickTarget();

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

    QVector3D rightClickTarget;      // 右键点击的目标点
    bool hasRightClickTarget = false; // 是否有效目标点
    QVector3D leftClickEye;          // 左键按下时的相机位置

    const float panSensitivity = 0.01f;    // 平移灵敏度
    const float earthRadius = 6371.0f;        // 地球半径（根据实际情况调整）
    

};