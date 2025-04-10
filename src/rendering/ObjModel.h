#pragma once

/**
 * @class ObjModel
 * @brief 简易 .obj 模型加载与渲染器，支持三角面片绘制。
 *
 * ObjModel 负责加载 Wavefront `.obj` 格式的静态三维模型，
 * 提取其中的顶点与法向量数据，并提供统一的渲染接口 render()，
 * 使用 OpenGL 的 immediate 模式绘制三角面。
 *
 * ◆ 功能职责：
 * - 加载指定路径的 `.obj` 文件，解析顶点与法线；
 * - 存储三角面顶点数据并统计面片数量；
 * - 提供 render() 方法，在绑定模型变换后绘制；
 *
 * ◆ 与其他模块的关系：
 * - 被 Renderer 持有，通常用于渲染飞机、武器平台等模型；
 * - 不处理位姿与缩放，仅负责“如何绘制”模型本身；
 * - 可重复使用，支持动态加载不同模型资源；
 *
 * ◆ 模块归属：
 * 本类归属于 rendering 模块，专注于模型资源的加载与绘制，
 * 与 UI、交互、数据模块解耦，符合单一职责与高内聚设计原则。
 */

 
#include <QString>
#include <QVector>
#include <QVector3D>

class ObjModel {
public:
    bool load(const QString& filename);
    void render();

private:
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    int faceCount = 0;
};