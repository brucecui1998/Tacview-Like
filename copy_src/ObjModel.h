#pragma once

#include <QString>
#include <QVector>
#include <QVector3D>

class ObjModel {
public:
    bool load(const QString &filename);
    void render();

private:
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    int faceCount = 0;
};
