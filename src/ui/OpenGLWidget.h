#pragma once

/**
 * @class OpenGLWidget
 * @brief 系统主绘图窗口，负责用户交互、渲染控制与播放调度。
 *
 * OpenGLWidget 是 UI 控制模块的核心类，继承自 QOpenGLWidget，
 * 用于显示 3D 画面、响应用户输入事件（鼠标拖动、滚轮缩放），
 * 并调度各个核心模块（相机控制、场景状态、渲染器、HUD）完成画面更新。
 *
 * 它本质上是连接“用户”与“系统核心”的桥梁，用户点击“播放”，选择 ACMI 文件、
 * 拖动视角等，所有操作都会通过这个类驱动系统运行。
 *
 * ◆ 与其他模块的关系：
 * - [SceneManager]：用于管理当前时间帧，获取目标状态（控制播放节奏）；
 * - [Renderer]：用于渲染地球和飞机等 3D 对象；
 * - [MouseCameraController]：处理鼠标输入，控制相机位置与朝向；
 * - [HudRenderer]：绘制 HUD 元素，如指南针（固定在屏幕上的 UI）；
 *
 * ◆ 核心职责：
 * - 初始化 OpenGL 上下文；
 * - 每帧触发 paintGL() 完成画面更新；
 * - 控制播放时间推进；
 * - 统一触发渲染器与视角设置；
 * - 响应用户输入（鼠标旋转、缩放）；
 *
 * 本类是系统视觉层的主驱动器，对用户“看见”和“操作”的一切负责。
 */

 
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>  // ✅ 或 2_0, 3_0, 4_1 取决于你的上下文版本
#include <QTimer>
#include <QElapsedTimer>
#include <QMouseEvent>
#include "scene/SceneManager.h"
#include "rendering/Renderer.h"
#include "platform/MouseCameraController.h"
#include "platform/OrbitalCameraController.h"
#include "ui/HudRenderer.h"  

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    void setFlightDataProvider(IFlightDataProvider* provider);

public slots:
    void startPlayback();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    SceneManager scene;
    Renderer renderer;
    OrbitalCameraController camera;
    HudRenderer hudRenderer;

    QTimer playbackTimer;
    QElapsedTimer playbackClock;
    QList<double> frameTimes;
    int frameIndex = 0;
    double currentTime = 0.0;
};
