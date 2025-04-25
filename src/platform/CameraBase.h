#pragma once

#include <QMatrix4x4>
#include <QVector3D>

class CameraBase {
public:
    virtual ~CameraBase() = default;

    // 设置视图矩阵
    virtual void applyCamera() = 0;

    // 鼠标事件
    virtual void mousePress(int x, int y, Qt::MouseButtons buttons) = 0;
    virtual void mouseRelease(int x, int y, Qt::MouseButtons buttons) = 0;
    virtual void mouseMove(int x, int y, Qt::MouseButtons buttons) = 0;
    virtual void wheelZoom(int delta) = 0;

    // 获取观察点信息（可选）
    virtual QVector3D getEye() const = 0;
    virtual float getRotationY() const = 0;
};