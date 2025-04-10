#pragma once

/**
 * @class MouseCameraController
 * @brief 鼠标视角控制器，处理相机旋转与缩放的输入响应。
 *
 * MouseCameraController 专门负责将用户鼠标输入（拖动与滚轮）
 * 转换为 3D 相机的视角变换（旋转 + 缩放），并通过 applyCamera()
 * 统一设置 OpenGL 的观察矩阵（gluLookAt）。
 *
 * ◆ 为什么属于 platform 层？
 * 该类与 UI 控件、数据内容无关，仅处理“输入事件 → 相机状态”的逻辑，
 * 具有平台无关性（可用于任何使用 OpenGL 的窗口系统，如 Qt 或 GLFW），
 * 因此抽象为平台适配模块的一部分，便于未来移植、复用与替换。
 *
 * ◆ 功能职责：
 * - 鼠标左键拖动：控制视角旋转
 * - 鼠标滚轮缩放：控制相机远近
 * - 相机矩阵绑定：通过 applyCamera() 应用到 OpenGL 观察矩阵
 *
 * ◆ 使用场景：
 * 被 OpenGLWidget 控制调用，在每帧绘制前调用 applyCamera()
 * 设置当前视角；在 mouseEvent 中调用 mousePress/mouseMove/wheelZoom 响应输入。
 */


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
    float getRotationY() const;

private:
    QPoint lastPos;
    float rotX = 0.0f;
    float rotY = 0.0f;
    float distance = 1.5f;
};