#pragma once

#include <QMap>
#include <QString>
#include <QVector3D>
#include <QList>

// ---------- 数据结构定义 ----------
struct ObjectState {
    QVector3D position;
    double pitch;
    double roll;
    double yaw;
};

// ---------- 提供统一数据接口 ----------
class IFlightDataProvider {
public:
    virtual ~IFlightDataProvider() = default;
    virtual bool loadFromFile(const QString &filePath) = 0;
    virtual QMap<QString, ObjectState> getObjectStatesAt(double time) const = 0;
    virtual QList<double> getAllTimeFrames() const = 0;
};
