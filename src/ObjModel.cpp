#include <windows.h>    // ✅ 必须在最前
#include "ObjModel.h"
#include "thirdparty/tiny_obj_loader.h"
#include <QFile>
#include <QDebug>
#include <GL/gl.h>

bool ObjModel::load(const QString &filename) {
    vertices.clear();
    normals.clear();

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.toStdString().c_str());

    if (!warn.empty()) qDebug() << "[TinyObj] warning:" << QString::fromStdString(warn);
    if (!err.empty()) qWarning() << "[TinyObj] error:" << QString::fromStdString(err);
    if (!ret) return false;

    for (const auto& shape : shapes) {
        for (const auto& idx : shape.mesh.indices) {
            int vi = 3 * idx.vertex_index;

            QVector3D v(
                attrib.vertices[vi + 0],
                attrib.vertices[vi + 1],
                attrib.vertices[vi + 2]);
            vertices.append(v);

            // ✅ 判断是否存在合法法线
            if (idx.normal_index >= 0 &&
                size_t(3 * idx.normal_index + 2) < attrib.normals.size()) {
                int ni = 3 * idx.normal_index;
                QVector3D n(
                    attrib.normals[ni + 0],
                    attrib.normals[ni + 1],
                    attrib.normals[ni + 2]);
                normals.append(n);
            }
            else {
                // 没有法线就用默认值（如向上）
                normals.append(QVector3D(0, 0, 1));
            }
        }
    }


    faceCount = vertices.size() / 3;
    if (!ret) {
        qWarning() << "[TinyObj] failed to load:" << filename;
        return false;
    }

    qDebug() << "[TinyObj] vertices:" << vertices.size()
        << "faces:" << faceCount
        << "normals:" << normals.size();
    return true;
}

void ObjModel::render() {
    if (vertices.isEmpty()) {
        qWarning() << "[Model] No vertex data to render.";
        return;
    }

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < vertices.size(); ++i) {
        glNormal3f(normals[i].x(), normals[i].y(), normals[i].z());
        glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
    }
    glEnd();
}

