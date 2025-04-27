#include "FlightData.h"

void FlightData::addObjectState(double time, const QString &id, const ObjectState &state) {
    timeline[time][id] = state;
}

QMap<QString, ObjectState> FlightData::getObjectStatesAt(double time) const {
    // ✅ 增加状态累积逻辑
    QMap<QString, ObjectState> accumulated;

    for (auto it = timeline.constBegin(); it != timeline.constEnd(); ++it) {
        if (it.key() > time) break;

        const QMap<QString, ObjectState>& frame = it.value();
        for (auto objIt = frame.constBegin(); objIt != frame.constEnd(); ++objIt) {
            accumulated[objIt.key()] = objIt.value(); // 覆盖或更新对象状态
        }
    }

    return accumulated;
}

QList<double> FlightData::getAllTimeFrames() const {
    return timeline.keys();
}