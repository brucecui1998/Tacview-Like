#include "EarthSphere.h"
#include <QDir>
#include <QImage>
#include <QDebug>
#include <gl/GLU.h>

EarthSphere::EarthSphere() {}
EarthSphere::~EarthSphere() {}

void EarthSphere::init() {
    initializeOpenGLFunctions();
    setSphereQuality(SphereQuality::Medium);
    loadTexture();
}

void EarthSphere::setSphereQuality(SphereQuality quality) {
    switch (quality) {
        case SphereQuality::Low:
            generateSphere(180, 360);  // 性能优先
            break;
        case SphereQuality::Medium:
            generateSphere(1024, 2048);  // 平衡视觉和性能
            break;
        case SphereQuality::High:
            generateSphere(2048, 4096);  // 高清效果
            break;
    }
}

// 在 3D 空间中按经纬度网格生成一个单位球体的顶点和纹理坐标
// 也就是你看到的地球球面模型（用于绘制地球贴图）
// 1、按纬度（stacks）和经度（slices）划分球面网格
// 2、计算每个顶点的三维坐标 (x, y, z)
// 3、计算每个顶点对应的纹理坐标 (u, v)（贴地球贴图用）
void EarthSphere::generateSphere(int stacks_, int slices_) {
    vertices.clear();
    texCoords.clear();
    const float PI = 3.1415926f;

    stacks = stacks_;
    slices = slices_;

    for (int i = 0; i <= stacks; ++i) {
        float lat = PI * (-0.5f + (float)i / stacks);
        float z = sin(lat);
        float zr = cos(lat);

        for (int j = 0; j <= slices; ++j) {
            float lng = 2 * PI * (float)j / slices;
            float x = zr * cos(lng);
            float y = zr * sin(lng);
            float u = (float)j / slices;
            float v = 1.0f - (float)i / stacks;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            texCoords.push_back(u);
            texCoords.push_back(v);
        }
    }
}

void EarthSphere::loadTexture() {
    QString path = QDir::currentPath() + "/resources/earth.jpg";
    QImage image(path);
    if (image.isNull()) {
        QImage fallback(64, 64, QImage::Format_RGBA8888);
        fallback.fill(Qt::white);
        image = fallback;
    }

    QImage tex = image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLfloat maxAniso = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
}

void EarthSphere::render() {
    if (!textureId) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);

    for (int i = 0; i < stacks; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            int idx1 = i * (slices + 1) + j;
            int idx2 = (i + 1) * (slices + 1) + j;

            glTexCoord2f(texCoords[2 * idx1], texCoords[2 * idx1 + 1]);
            glVertex3f(vertices[3 * idx1], vertices[3 * idx1 + 1], vertices[3 * idx1 + 2]);

            glTexCoord2f(texCoords[2 * idx2], texCoords[2 * idx2 + 1]);
            glVertex3f(vertices[3 * idx2], vertices[3 * idx2 + 1], vertices[3 * idx2 + 2]);
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
}

GLuint EarthSphere::getTextureId() const {
    return textureId;
}
