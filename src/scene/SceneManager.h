#pragma once

/**
 * @class SceneManager
 * @brief 场景管理模块，用于管理当前播放时间与对象状态调度。
 *
 * 该类作为 UI 与数据模块之间的中间层，接收 IFlightDataProvider 数据源，
 * 并根据当前时间戳提供当前帧的目标状态集合。所有渲染或 UI 层无需关心
 * 数据存储细节，只需要通过本类获取时间帧对应的对象状态即可。
 */

#include "core/IFlightDataProvider.h"
#include <QMap>
#include <QString>

class SceneManager {
public:
    void bindDataProvider(IFlightDataProvider* provider);
    void updateCurrentTime(double time);

    QMap<QString, ObjectState> getCurrentObjectStates() const;
    QList<double> getTimeFrames() const;
    double getCurrentTime() const;

private:
    IFlightDataProvider* dataProvider = nullptr;
    double currentTime = 0.0;
};
