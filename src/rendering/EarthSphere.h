#pragma once

/**
 * @class EarthSphere
 * @brief 地球球体渲染模块，生成带贴图的 3D 球面模型。
 *
 * EarthSphere 是渲染模块中的地理基础元素，负责构建一个球体
 * 并加载地球纹理贴图，实现贴图地球在 3D 空间中的可视化。
 *
 *
 * 在实际显示时，观察距离由相机控制（如 MouseCameraController），球体的视觉比例通过 OpenGL 缩放或视角决定。
 *
 * ◆ 职责说明：
 * - 使用经纬度方式生成球体顶点（generateSphere）
 * - 加载地球纹理贴图并创建 OpenGL 纹理（loadTexture）
 * - 提供 render() 接口供 Renderer 渲染地球球面
 *
 * ◆ 与其他模块关系：
 * - 被 Renderer 控制使用，通常在 renderEarth() 内部调用；
 * - 不管理场景数据或动画，仅生成静态贴图球体模型；
 */


#include <QOpenGLFunctions>
#include <QImage>
#include <vector>

enum class SphereQuality {
    Low,
    Medium,
    High
};

class EarthSphere : protected QOpenGLFunctions {
public:
    EarthSphere();
    ~EarthSphere();

    void init();
    void render();
    GLuint getTextureId() const;
    // 地球真实半径常量（单位：米）
    static constexpr float EARTH_RADIUS = 6371000.0f;


private:
    void generateSphere(int stacks, int slices);
    void loadTexture();
    void setSphereQuality(SphereQuality quality);

    GLuint textureId = 0;
    std::vector<float> vertices;
    std::vector<float> texCoords;
    int stacks = 0;//(纬度方向)	贴图高度 → 从南极到北极
    int slices = 0;//(经度方向)	贴图宽度 → 围绕赤道一圈
};
