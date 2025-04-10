#include "AcmiParser.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QtMath>

AcmiParser::AcmiParser(const QString &filePath) : m_filePath(filePath) {}

bool AcmiParser::parse() {
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        parseLine(line);
    }
    return true;
}

bool AcmiParser::loadFromFile(const QString &filePath) {
    m_filePath = filePath;
    return parse();
}

QMap<QString, ObjectState> AcmiParser::getObjectStatesAt(double time) const {
    return m_flightData.getObjectStatesAt(time);
}

QList<double> AcmiParser::getAllTimeFrames() const {
    return m_flightData.getAllTimeFrames();
}

void AcmiParser::parseLine(const QString &line) {
    if (line.startsWith("#")) {
        parseFrameMarker(line);
    } else if (line.contains(",T=")) {
        parseObjectState(line);
    }
    // 可选元信息行略过
}

void AcmiParser::parseFrameMarker(const QString &line) {
    QString timeStr = line.mid(1);
    double rawTime = timeStr.toDouble();
    currentTime = std::round(rawTime * 100.0) / 100.0;
}

void AcmiParser::parseObjectState(const QString &line) {
    QStringList parts = line.split(",", Qt::SkipEmptyParts);
    if (parts.size() < 2) return;

    QString objectId = parts[0];
    QStringList keyValues = parts[1].split("=");
    if (keyValues.size() < 2) return;

    QStringList values = keyValues[1].split("|");
    if (values.size() < 6) return;

    ObjectState state;
    state.position.setX(values[0].toDouble());
    state.position.setY(values[1].toDouble());
    state.position.setZ(values[2].toDouble());

    state.pitch = values[3].toDouble();
    state.roll  = values[4].toDouble();
    state.yaw   = values[5].toDouble();

    m_flightData.addObjectState(currentTime, objectId, state);
}
