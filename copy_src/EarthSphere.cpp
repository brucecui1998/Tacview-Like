#include "EarthSphere.h"
#include <cmath>
#include <QDebug>
#include <QDir>
#include <gl/GLU.h>

EarthSphere::EarthSphere() : textureId(0) {}
EarthSphere::~EarthSphere() {}

void EarthSphere::init() {
    initializeOpenGLFunctions();
    generateSphere(60, 120);  // 越大越平滑
    loadTexture();
}

void EarthSphere::generateSphere(int stacks, int slices) {
    vertices.clear();
    texCoords.clear();
    const float PI = 3.1415926f;

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
    qDebug() << "[EarthSphere] Trying to load texture from:" << path;

    QImage image(path);
    if (image.isNull()) {
        qWarning() << "[EarthSphere] Failed to load earth texture image!";
        qWarning() << "[EarthSphere] Trying fallback image (white)...";

        // 创建一个纯白图像作为 fallback
        QImage fallback(64, 64, QImage::Format_RGBA8888);
        fallback.fill(Qt::white);
        image = fallback;
    }

    QImage tex = image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // 使用 Mipmap 支持
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA,
        tex.width(), tex.height(),
        GL_RGBA, GL_UNSIGNED_BYTE,
        tex.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    qDebug() << "[EarthSphere] Texture loaded successfully. Size:"
        << tex.width() << "x" << tex.height();
}

void EarthSphere::render() {
    if (!textureId) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);

    int stacks = 60;
    int slices = 120;
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