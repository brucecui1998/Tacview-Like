#include "Renderer.h"
#include "Config.h"
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

QVector3D geoToSphere(double latDeg, double lonDeg, double radius) {
    double lat = qDegreesToRadians(latDeg);
    double lon = qDegreesToRadians(lonDeg);
    return QVector3D(
        radius * qCos(lat) * qCos(lon),
        radius * qCos(lat) * qSin(lon),
        radius * qSin(lat)
    );
}

// ✅ 使用单位球 + 高度放大因子（视觉增强）
void Renderer::renderScene(const SceneManager& scene, const QVector3D& eye) {
    QMap<QString, ObjectState> stateMap = scene.getCurrentObjectStates();
    if (!stateMap.contains("A0100")) {
        //qDebug() << "[Renderer] No state found for A0100";
        return;
    }

    const ObjectState& state = stateMap["A0100"];

    double lon = state.position.x();
    double lat = state.position.y();
    double alt = state.position.z();

    // qDebug().noquote() << QString("[Renderer] Pos: lon=%1  lat=%2  alt=%3")
    //                       .arg(lon, 0, 'f', 10)
    //                       .arg(lat, 0, 'f', 10)
    //                       .arg(alt, 0, 'f', 10);

    QVector3D pos = geoToSphere(lat, lon, EARTH_RADIUS + HEIGHT_EXAGGERATION * alt / 6371000.0);

    //qDebug() << "[Renderer] Converted 3D position:" << pos;

    static QVector3D lastPos;
    //qDebug() << "[Renderer] Delta = " << (pos - lastPos).length();
    lastPos = pos;

    //qDebug() << "[Camera] Eye =" << eye;
    //qDebug() << "[Camera → Aircraft Distance] =" << (pos - eye).length();
    //qDebug() << "[Renderer] Distance from center:" << pos.length();

     // ✅ 绘制轨迹线
     static std::vector<QVector3D> trajectory;
     trajectory.push_back(pos);
     glPushAttrib(GL_CURRENT_BIT);  // ✅ 保留颜色状态
     glColor3f(1.0f, 1.0f, 0.0f);
     glBegin(GL_LINE_STRIP);
     for (const auto& p : trajectory) {
         glVertex3f(p.x(), p.y(), p.z());
     }
     glEnd();
     glPopAttrib();  // ✅ 恢复颜色状态
 

    // ✅ 绘制飞机模型
    glPushMatrix();
    glTranslatef(pos.x(), pos.y(), pos.z());

    glRotatef(state.yaw,   0, 0, 1);
    glRotatef(state.pitch, 1, 0, 0);
    glRotatef(state.roll,  0, 1, 0);
    glRotatef(-90, 1, 0, 0);
    glRotatef(180, 0, 0, 1);
    glScalef(AIRCRAFT_SCALE, AIRCRAFT_SCALE, AIRCRAFT_SCALE);

    //qDebug() << "[Renderer] Rendering aircraft model at" << pos;
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(1, 0, 0);
    aircraftModel.render();
    glPopAttrib();

    glPopMatrix();
}
#include <QMatrix4x4>
// void Renderer::renderScene(const SceneManager& scene) {

//     QMap<QString, ObjectState> stateMap = scene.getCurrentObjectStates();
//     if (!stateMap.contains("A0100")) {
//         qDebug() << "[Renderer] No state found for A0100";
//         return;
//     }

//     const ObjectState& state = stateMap["A0100"];

//     double lon = state.position.x();
//     double lat = state.position.y();
//     double alt = state.position.z();

//     qDebug().noquote() << QString("[Renderer] Pos: lon=%1  lat=%2  alt=%3")
//                           .arg(lon, 0, 'f', 10)
//                           .arg(lat, 0, 'f', 10)
//                           .arg(alt, 0, 'f', 10);

//     constexpr double EARTH_RADIUS = 6371.0;
//     QVector3D pos = geoToSphere(lat, lon, EARTH_RADIUS + alt / 1000.0);

//     glLoadIdentity();  // 清除旧的 MODELVIEW 矩阵

//     QVector3D eye = pos + QVector3D(0, -2000, 1000);  // 从飞机后方观察
//     gluLookAt(eye.x(), eye.y(), eye.z(),
//               pos.x(), pos.y(), pos.z(),              // 观察点对准飞机
//               0, 0, 1);
          
              
//     qDebug() << "[Renderer] Converted 3D position:" << pos;

//     static QVector3D lastPos;
//     qDebug() << "[Renderer] Delta = " << (pos - lastPos).length();
//     lastPos = pos;

//     qDebug() << "[Camera] Eye =" << eye;
//     qDebug() << "[Camera → Aircraft Distance] =" << (pos - eye).length();
//     qDebug() << "[Renderer] Distance from center:" << pos.length();

//     glDisable(GL_DEPTH_TEST);   // 🔥 禁用深度测试，保证飞机覆盖地球
//     // ✅ 绘制飞机模型（仍然可选）
//     glPushMatrix();
//     glTranslatef(pos.x(), pos.y(), pos.z());
//     glRotatef(state.yaw,   0, 0, 1);
//     glRotatef(state.pitch, 1, 0, 0);
//     glRotatef(state.roll,  0, 1, 0);
//     glRotatef(-90, 1, 0, 0);
//     glRotatef(180, 0, 0, 1);
//     glScalef(500.0f, 500.0f, 500.0f);  // 测试原始尺寸模型是否能看到

//     glPushAttrib(GL_CURRENT_BIT);
//     glColor3f(1, 0, 0);
//     qDebug() << "[Renderer] Rendering aircraft model at" << pos;
//     aircraftModel.render();
//     glPopAttrib();
//     glPopMatrix();

//     glEnable(GL_DEPTH_TEST);    // ✅ 恢复深度测试
// }


void Renderer::renderEarth() {
    // //透明渲染地球
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glColor4f(1.0, 1.0, 1.0, 0.2f);  // 设置地球透明度为 20%


    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, earth.getTextureId());
    earth.render();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
