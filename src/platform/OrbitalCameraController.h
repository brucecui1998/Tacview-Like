#pragma once

#include "CameraBase.h"
#include <QPoint>
#include <QtMath>
#include <QDebug>

class OrbitalCameraController : public CameraBase {
public:
    OrbitalCameraController();

    void mousePress(int x, int y, Qt::MouseButtons buttons) override;
    void mouseRelease(int x, int y, Qt::MouseButtons buttons) override;
    void mouseMove(int x, int y, Qt::MouseButtons buttons) override;
    void wheelZoom(int delta) override;
    void applyCamera() override;

    QVector3D getEye() const override;
    float getRotationY() const override;

private:
    QPoint lastPos;
    float distance;
    float yaw;
    float pitch;
    float sensitivity;
    float zoomSpeed;
    float elevation = 0.0f;  // 新增：右键控制高度（单位同 eye 坐标）


    QMatrix4x4 view;
    QVector3D eyePosition;
    Qt::MouseButtons currentButtons = Qt::NoButton;
};
