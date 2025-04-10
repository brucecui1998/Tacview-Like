#pragma once

/**
 * @class AcmiParser
 * @brief ACMI 数据文件解析器，实现统一数据访问接口。
 *
 * AcmiParser 负责读取并解析 Tacview 导出的 .acmi 格式文本文件，
 * 将其中的目标状态信息解析为标准化的 FlightData 数据结构，并通过
 * IFlightDataProvider 接口对外提供统一的时间帧 → 对象状态访问方式。
 *
 * 本类是系统的数据入口，供 SceneManager 或其他播放控制模块绑定使用。
 *
 * 使用方式：
 * 1. 实例化时传入文件路径
 * 2. 调用 parse() 或 loadFromFile()
 * 3. 通过接口获取任意时间帧的对象状态
 */

#include <QString>
#include "FlightData.h"
#include "IFlightDataProvider.h"

class AcmiParser : public IFlightDataProvider {
public:
    explicit AcmiParser(const QString &filePath);
    bool parse();

    // IFlightDataProvider 接口实现
    bool loadFromFile(const QString &filePath) override;
    QMap<QString, ObjectState> getObjectStatesAt(double time) const override;
    QList<double> getAllTimeFrames() const override;

private:
    QString m_filePath;
    FlightData m_flightData;

    void parseLine(const QString &line);
    void parseFrameMarker(const QString &line);
    void parseObjectState(const QString &line);

    double currentTime = 0.0;
};