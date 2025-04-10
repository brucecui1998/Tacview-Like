#include "FlightData.h"

void FlightData::addObjectState(double time, const QString &id, const ObjectState &state) {
    timeline[time][id] = state;
}

QMap<QString, ObjectState> FlightData::getObjectStatesAt(double time) const {
    return timeline.value(time);
}

QList<double> FlightData::getAllTimeFrames() const {
    return timeline.keys();
}