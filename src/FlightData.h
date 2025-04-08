#pragma once

#include <QMap>
#include <QString>
#include <QVector3D>

struct ObjectState {
    QVector3D position;
    double pitch;
    double roll;
    double yaw;
};

class FlightData {
public:
    void addObjectState(double time, const QString &id, const ObjectState &state);
    QMap<QString, ObjectState> getObjectStatesAt(double time) const;
    QList<double> getAllTimeFrames() const;

private:
    QMap<double, QMap<QString, ObjectState>> timeline; // time -> (id -> state)
};