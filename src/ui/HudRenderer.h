#pragma once

/**
 * @class HudRenderer
 * @brief HUD（抬头显示）渲染器，用于绘制屏幕固定辅助元素，如指南针。
 *
 * HudRenderer 负责绘制不属于 3D 场景的 UI 元素，常用于飞行可视化中的辅助信息展示。
 * 这些元素始终固定在屏幕上的某个位置，例如右上角的指南针，不会随视角、缩放而改变。
 * 
 * - 为什么叫 HudRenderer？
 *   HUD 是 Heads-Up Display 的缩写，源于航空领域，表示“抬头显示”信息层；
 *   例如指南针、速度表、高度尺、目标标签等都属于 HUD 的典型元素。
 *
 * - 为什么不放在 Renderer 中？
 *   与 Renderer 专注绘制 3D 场景（地球、飞机）不同，HudRenderer 使用的是 2D 投影（glOrtho），
 *   并且不受相机视角控制，为了职责分离、避免状态冲突和便于扩展，单独封装为一个模块。
 *
 * - 是否只负责指南针？
 *   当前仅绘制指南针（drawCompass），但架构上为将来添加其他 HUD 元素（如速度表、姿态球）预留了空间。
 */

#include <QOpenGLFunctions_3_3_Core>  // ✅ 或 2_0, 3_0, 4_1 取决于你的上下文版本

class HudRenderer : protected QOpenGLFunctions_3_3_Core {
public:
    void drawCompass(float cameraRotationY, int width, int height);
};
