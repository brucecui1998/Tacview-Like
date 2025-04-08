#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QImage>

class EarthSphere : protected QOpenGLFunctions {
public:
    EarthSphere();
    ~EarthSphere();

    void init();
    void render();
    GLuint getTextureId() const { return textureId; }


private:
    void generateSphere(int stacks, int slices);
    void loadTexture();

    GLuint textureId;
    std::vector<float> vertices;
    std::vector<float> texCoords;
};