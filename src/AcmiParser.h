#pragma once

#include <QString>
#include "FlightData.h"

class AcmiParser {
public:
    explicit AcmiParser(const QString &filePath);
    bool parse();
    const FlightData &getFlightData() const;

private:
    QString m_filePath;
    FlightData m_flightData;

    void parseLine(const QString &line);
    void parseFrameMarker(const QString &line);
    void parseObjectState(const QString &line);

    double currentTime = 0.0;
};