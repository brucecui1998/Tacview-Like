#pragma once

#include "IFlightDataProvider.h"

class FlightData {
public:
    void addObjectState(double time, const QString &id, const ObjectState &state);
    QMap<QString, ObjectState> getObjectStatesAt(double time) const;
    QList<double> getAllTimeFrames() const;

private:
    QMap<double, QMap<QString, ObjectState>> timeline; // time -> (id -> state)
};