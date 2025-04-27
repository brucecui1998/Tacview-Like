#include <windows.h>
#include "ObjModel.h"
#include "thirdparty/tiny_obj_loader/tiny_obj_loader.h"
#include <QDebug>
#include <GL/gl.h>

bool ObjModel::load(const QString& filename) {
    vertices.clear();
    normals.clear();

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.toStdString().c_str());

    if (!ret) return false;

    float minX = FLT_MAX, maxX = -FLT_MAX;
    float minY = FLT_MAX, maxY = -FLT_MAX;
    float minZ = FLT_MAX, maxZ = -FLT_MAX;

    for (const auto& shape : shapes) {
        for (const auto& idx : shape.mesh.indices) {
            int vi = 3 * idx.vertex_index;
            QVector3D v(
                attrib.vertices[vi + 0],
                attrib.vertices[vi + 1],
                attrib.vertices[vi + 2]);

            vertices.append(v);

            // 计算包围盒
            minX = qMin(minX, v.x()); maxX = qMax(maxX, v.x());
            minY = qMin(minY, v.y()); maxY = qMax(maxY, v.y());
            minZ = qMin(minZ, v.z()); maxZ = qMax(maxZ, v.z());

            //qDebug() << "[ObjModel] Model height before scaling:" << (maxY - minY);

            if (idx.normal_index >= 0 && size_t(3 * idx.normal_index + 2) < attrib.normals.size()) {
                int ni = 3 * idx.normal_index;
                normals.append(QVector3D(
                    attrib.normals[ni + 0],
                    attrib.normals[ni + 1],
                    attrib.normals[ni + 2]
                ));
            } else {
                normals.append(QVector3D(0, 0, 1));
            }
        }
    }

    faceCount = vertices.size() / 3;

    // ✅ 以机腹（Y轴最低点）为原点进行对齐归一化
    float maxExtent = qMax(qMax(maxX - minX, maxY - minY), maxZ - minZ);
    float scale = 1.0f / maxExtent;

    for (int i = 0; i < vertices.size(); ++i) {
        // 平移到底部对齐 + 缩放
        vertices[i].setX((vertices[i].x() - (minX + maxX) / 2) * scale); // X轴仍然居中
        vertices[i].setY((vertices[i].y() - minY) * scale);              // ✅ Y轴底部对齐
        vertices[i].setZ((vertices[i].z() - (minZ + maxZ) / 2) * scale); // Z轴仍然居中
    }

    qDebug() << "[ObjModel] Normalized and loaded vertices:" << vertices.size();
    return true;
}

void ObjModel::render() {
    if (vertices.isEmpty()) return;
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < vertices.size(); ++i) {
        glNormal3f(normals[i].x(), normals[i].y(), normals[i].z());
        glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
    }
    glEnd();
}
