#include <QVector3D>
#include <QMatrix4x4>

class RayCaster {
public:
    static bool castToEarth(int screenX, int screenY, float earthRadius, QVector3D& outIntersection) {
        // 获取当前视口和矩阵
        GLint viewport[4];
        GLdouble modelview[16], projection[16];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        
        // 计算射线
        GLdouble startX, startY, startZ;
        GLdouble endX, endY, endZ;
        gluUnProject(screenX, viewport[3]-screenY, 0, 
                    modelview, projection, viewport, 
                    &startX, &startY, &startZ);
        gluUnProject(screenX, viewport[3]-screenY, 1, 
                    modelview, projection, viewport, 
                    &endX, &endY, &endZ);
        
        // 射线与地球求交
        QVector3D rayOrigin(startX, startY, startZ);
        QVector3D rayDir(endX-startX, endY-startY, endZ-startZ);
        rayDir.normalize();
        
        return sphereIntersection(rayOrigin, rayDir, QVector3D(0,0,0), earthRadius, outIntersection);
    }

private:
    static bool sphereIntersection(const QVector3D& origin, const QVector3D& dir,
                                  const QVector3D& center, float radius, QVector3D& out) {
        QVector3D oc = origin - center;
        float a = QVector3D::dotProduct(dir, dir);
        float b = 2.0 * QVector3D::dotProduct(oc, dir);
        float c = QVector3D::dotProduct(oc, oc) - radius*radius;
        float discriminant = b*b - 4*a*c;
        
        if (discriminant < 0) return false;
        
        float t = (-b - sqrt(discriminant)) / (2.0*a);
        out = origin + t*dir;
        return true;
    }
};