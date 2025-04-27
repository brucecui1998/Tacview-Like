#pragma once

#include "CameraBase.h"
#include <QPoint>
#include <QtMath>
#include <QDebug>
#include <algorithm>

class OrbitalCameraController : public CameraBase {
public:
    OrbitalCameraController();
    void setPitchAngle(float degrees); // 新增：设置俯仰角
    float getPitchAngle() const;

    void mousePress(int x, int y, Qt::MouseButtons buttons) override;
    void mouseRelease(int x, int y, Qt::MouseButtons buttons) override;
    void mouseMove(int x, int y, Qt::MouseButtons buttons) override;
    void wheelZoom(int delta) override;
    void applyCamera() override;

    QVector3D getEye() const override;
    float getRotationY() const override;
    void setViewportSize(int width, int height);

private:
    float distance;      // 相机到地球中心的真实距离
    float yaw;          // 水平旋转角度（0-360度）
    float pitch;        // 俯仰角度（-89~89度）

    QPoint lastPos;
    float sensitivity;   // 鼠标灵敏度
    float zoomSpeed;     // 缩放速度

    QMatrix4x4 view;
    QVector3D eyePosition;
    Qt::MouseButtons currentButtons = Qt::NoButton;
    int viewportWidth = 1920;  // 默认值
    int viewportHeight = 1080; // 默认值
};