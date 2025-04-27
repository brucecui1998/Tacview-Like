#pragma once

/**
 * @class Renderer
 * @brief 渲染模块核心类，负责 OpenGL 场景绘制任务。
 *
 * Renderer 是整个系统的可视化核心，封装了 OpenGL 初始化与渲染流程，
 * 负责绘制地球贴图球体、目标模型（如飞机）、并支持视角变化下的稳定显示。
 *
 * 该类管理渲染子模块 EarthSphere（地球）与 ObjModel（飞机），
 * 对外提供统一的渲染接口，包括 renderEarth() 和 renderScene()。
 *
 * 与其他模块的关系：
 * - 接收 SceneManager 提供的当前时间帧对象状态（如位置、姿态）；
 * - 渲染逻辑由 OpenGLWidget 控制调用（调用顺序决定渲染效果）；
 * - 不管理时间，不处理数据，只关注可视化；
 * - 可与 HudRenderer 共同协作实现完整画面显示。
 */

#include "scene/SceneManager.h"
#include "EarthSphere.h"
#include "ObjModel.h"
#include <QOpenGLFunctions>
#include <QMap>
#include <QString>

class Renderer : protected QOpenGLFunctions {
public:
    Renderer();
    void initialize();
    void renderEarth();
    void renderScene(const SceneManager& scene, const QVector3D& eye);
    

private:
    void drawObject(const ObjectState& state);
    void drawCompass(float cameraRotY);

    EarthSphere earth;
    ObjModel aircraftModel;
};