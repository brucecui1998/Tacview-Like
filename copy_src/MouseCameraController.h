#pragma once

#include <QPoint>
#include <QMatrix4x4>
#include <QVector3D>

class MouseCameraController {
public:
    MouseCameraController();

    void mousePress(int x, int y);
    void mouseMove(int x, int y);
    void wheelZoom(int delta);
    void applyCamera();
    float getRotationY() const { return rotY; }

private:
    QPoint lastPos;
    float rotX = 0.0f;
    float rotY = 0.0f;
    float distance = 2.5f;
};