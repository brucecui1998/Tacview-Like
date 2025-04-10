#include "Renderer.h"
#include <GL/glu.h>
#include <QDebug>
#include <QtMath>


Renderer::Renderer() {}

void Renderer::initialize() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    earth.init();
    if (!aircraftModel.load("resources/models/FixedWing.F-16.obj")) {
        qWarning() << "[Renderer] Failed to load model!";
    }
}

void Renderer::renderScene(const SceneManager& scene) {
    QMap<QString, ObjectState> stateMap = scene.getCurrentObjectStates();
    if (stateMap.contains("A0100")) {
        const ObjectState& state = stateMap["A0100"];

        double lon = state.position.x();
        double lat = state.position.y();
        double alt = state.position.z();

        QVector3D pos = QVector3D(
            qCos(qDegreesToRadians(lat)) * qCos(qDegreesToRadians(lon)),
            qCos(qDegreesToRadians(lat)) * qSin(qDegreesToRadians(lon)),
            qSin(qDegreesToRadians(lat))
        ) * (1.0 + alt / 100000.0);

        glPushMatrix();
        glTranslatef(pos.x(), pos.y(), pos.z());

        glRotatef(state.yaw,   0, 0, 1);
        glRotatef(state.pitch, 1, 0, 0);
        glRotatef(state.roll,  0, 1, 0);
        glRotatef(-90, 1, 0, 0);
        glRotatef(180, 0, 0, 1);
        glScalef(0.005f, 0.005f, 0.005f);

        glPushAttrib(GL_CURRENT_BIT);  // ✅ 保护当前颜色状态
        glColor3f(1, 0, 0);            // ✅ 设置飞机为红色
        aircraftModel.render();
        glPopAttrib();                 // ✅ 恢复之前的颜色状态

        glPopMatrix();
    }
}

void Renderer::renderEarth() {
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, earth.getTextureId());
    earth.render();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
